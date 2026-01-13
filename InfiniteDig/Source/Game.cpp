#include "Game.h"
#include "Game/Chunck/BlockSelector.h"
#include "Game/Pickaxe.h"
#include "Game/BackgroundManager.h"
#include "Utils/Manager/SoundManager.h"
#include "Utils/LayerShaker.h"
#include "UI/Toggle/ToggleUI.h"
#include "UI/Inventory/InventoryUI.h"
#include "UI/Shop/ShopUI.h"
#include "UI/Play/PlayUI.h"
#include "UI/Craft/CraftUI.h"
#include "UI/Setting/SettingUI.h"

USING_NS_AX;

bool Game::init()
{
    if (!Scene::initWithPhysics())
        return false;

    InitPhysics();
    RegisterService();

    CreateLayers();
    CreateInGame();
    CreateUIs();

    scheduleUpdate();
    return true;
}

void Game::onEnter()
{
    Scene::onEnter();
    SoundManager::getInstance()->PlayBGM("Sounds/BGM/Game BGM.mp3");
    m_start_system_y = m_system_layer->getPositionY() - 128.f;
}

void Game::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    m_inventory_service->Initialize();
}


void Game::update(float delta)
{
    float current_layer_y = m_system_layer->getPositionY();

    Size visible_size     = Director::getInstance()->getVisibleSize();
    float screen_center_y = visible_size.height / 2.0f;

    float target_y = screen_center_y - m_pickaxe->getPositionY();
    float depth_y  = target_y;
    if (target_y < 0)
    {
        target_y = 0;
    }

    Vec2 shaking_offset = static_cast<LayerShaker*>(m_layer_shaker)->GetOffset();
    float lerp_y = std::lerp(current_layer_y, target_y, 0.1f);
    Vec2 final_position = Vec2(shaking_offset.x, lerp_y + shaking_offset.y);
    m_system_layer->setPosition(final_position);

    float moved = depth_y - m_start_system_y;
    int depth   = static_cast<int>(moved / 32.f);
    depth       = std::max(0, depth);

    if (depth != m_current_depth)
    {
        m_current_depth = depth;
        static_cast<PlayUI*>(m_play_ui)->UpdateUI(depth);
    }
}



Layer* Game::GetLayer(LayerOrder layer) const
{
    switch (layer)
    {
    case LayerOrder::BACKGROUND_LAYER:
        return m_background_layer;

    case LayerOrder::SYSTEM_LAYER:
        return m_system_layer;

    case LayerOrder::GAMEOBJECT_LAYER:
        return m_gameobject_layer;

    case LayerOrder::UI_LAYER:
        return m_ui_layer;

    case LayerOrder::SETTING_LAYER:
        return m_setting_layer;

    default:
        return nullptr;
    }
}



void Game::InitPhysics()
{
    PhysicsWorld* physics_world = this->getPhysicsWorld();
    physics_world->setGravity(Vec2(0, -120.f));
}

void Game::RegisterService()
{
    m_inventory_service       = ServiceLocator::Get<Service::IInventoryService>();
    m_item_data_service       = ServiceLocator::Get<Service::IItemDataService>();
    m_block_data_service      = ServiceLocator::Get<Service::IBlockDataService>();
    m_background_data_service = ServiceLocator::Get<Service::IBackgroundDataService>();
    m_shop_data_service       = ServiceLocator::Get<Service::IShopDataService>();
    m_recipe_data_service     = ServiceLocator::Get<Service::IRecipeDataService>();
    m_setting_service         = ServiceLocator::Get<Service::ISettingService>();
}



void Game::CreateLayers()
{
    m_background_layer = Layer::create();
    m_system_layer     = Layer::create();
    m_gameobject_layer = Layer::create();
    m_ui_layer         = Layer::create();
    m_setting_layer    = Layer::create();

    this->addChild(m_background_layer, LayerOrder::BACKGROUND_LAYER);
    this->addChild(m_system_layer, LayerOrder::SYSTEM_LAYER);
    this->addChild(m_gameobject_layer, LayerOrder::GAMEOBJECT_LAYER);
    this->addChild(m_ui_layer, LayerOrder::UI_LAYER);
    this->addChild(m_setting_layer, LayerOrder::SETTING_LAYER);
}

void Game::CreateInGame()
{
    BackgroundManager* background_manager = BackgroundManager::create(m_background_data_service);
    m_background_layer->addChild(background_manager);

    BlockSelector* block_selector = BlockSelector::create(m_block_data_service);
    m_system_layer->addChild(block_selector);

    m_layer_shaker = LayerShaker::create();
    m_system_layer->addChild(m_layer_shaker);

    m_chunck_manager = ChunckManager::create(m_block_data_service,
                                             m_inventory_service,
                                             block_selector,
                                             background_manager);
    m_system_layer->addChild(m_chunck_manager);

    m_pickaxe = Pickaxe::create(static_cast<ChunckManager*>(m_chunck_manager));
    m_pickaxe->setPosition(Vec2(180, 500));
    m_system_layer->addChild(m_pickaxe);
}


void Game::CreateUIs()
{
    m_canvas_size = _director->getCanvasSize();

    m_play_ui = PlayUI::create(m_inventory_service,
                               m_item_data_service,
                               static_cast<Pickaxe*>(m_pickaxe),
                               static_cast<ChunckManager*>(m_chunck_manager),
                               static_cast<LayerShaker*>(m_layer_shaker));
    m_ui_layer->addChild(m_play_ui);

    m_inventory_ui = InventoryUI::create(m_inventory_service, m_item_data_service);
    m_ui_layer->addChild(m_inventory_ui);

    m_shop_ui = ShopUI::create(m_inventory_service, m_item_data_service, m_shop_data_service);
    m_ui_layer->addChild(m_shop_ui);

    m_craft_ui = CraftUI::create(m_recipe_data_service, m_item_data_service, m_inventory_service);
    m_ui_layer->addChild(m_craft_ui);

    m_setting_ui = SettingUI::create(m_setting_service);
    m_setting_layer->addChild(m_setting_ui);

    m_toggle_ui = ToggleUI::create(static_cast<PlayUI*>(m_play_ui),
                                   static_cast<InventoryUI*>(m_inventory_ui),
                                   static_cast<ShopUI*>(m_shop_ui),
                                   static_cast<SettingUI*>(m_setting_ui),
                                   static_cast<CraftUI*>(m_craft_ui),
                                   GetLayer(LayerOrder::SETTING_LAYER));
    m_ui_layer->addChild(m_toggle_ui);
}
