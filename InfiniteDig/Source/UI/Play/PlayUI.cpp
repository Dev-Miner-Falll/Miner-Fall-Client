#include "PlayUI.h"
#include "Game/Item/ItemActionContext.h"
#include "Game/Item/ItemActionStrategyFactory.h"

USING_NS_AX;

PlayUI* PlayUI::create(std::shared_ptr<Service::IInventoryService> inventory_service,
                       std::shared_ptr<Service::IItemDataService> item_data_service,
                       Pickaxe* pickaxe,
                       ChunckManager* chunck_manager,
                       LayerShaker* layer_shaker)
{
    PlayUI* play_ui = new (std::nothrow) PlayUI();
    if (play_ui && play_ui->init(inventory_service, item_data_service, pickaxe, chunck_manager, layer_shaker))
    {
        play_ui->autorelease();
        return play_ui;
    }

    AX_SAFE_DELETE(play_ui);
    return nullptr;
}

bool PlayUI::init(std::shared_ptr<Service::IInventoryService> inventory_service,
                  std::shared_ptr<Service::IItemDataService> item_data_service,
                  Pickaxe* pickaxe,
                  ChunckManager* chunck_manager,
                  LayerShaker* layer_shaker)
{
    if (!Layer::init())
        return false;

    m_inventory_service = inventory_service;
    m_item_data_service = item_data_service;
    m_pickaxe           = pickaxe;
    m_chunck_manager    = chunck_manager;
    m_layer_shaker      = layer_shaker;

    CreateUI();

    return true;
}



void PlayUI::OpenUI()
{
    ToggleUI(true);
}

void PlayUI::CloseUI()
{
    ToggleUI(false);
}

void PlayUI::ToggleUI(bool active)
{
    const float TARGET_TIME = 0.25f;

    if (active)
    {
        CallFunc* enable_action   = CallFunc::create([this]() { this->setVisible(true); });
        FadeIn* fade_in_action    = FadeIn::create(TARGET_TIME);
        Sequence* sequence_action = Sequence::create(enable_action, fade_in_action, nullptr);

        this->runAction(sequence_action);
    }
    else
    {
        FadeOut* fade_out_action  = FadeOut::create(TARGET_TIME);
        CallFunc* disable_action  = CallFunc::create([this]() { this->setVisible(false); });
        Sequence* sequence_action = Sequence::create(fade_out_action, disable_action, nullptr);

        this->runAction(sequence_action);
    }
}



void PlayUI::CreateUI()
{
    this->setCascadeOpacityEnabled(true);
    this->setOpacity(0.f);
    this->setVisible(false);

    m_canvas_size = _director->getCanvasSize();

    CreateQuickSlots();
    CreateDepthLabel();
}

void PlayUI::CreateQuickSlots()
{
    const float SLOT_SPACE = 8.f;
    const Vec2 SLOT_START_POSITION = Vec2(m_canvas_size.x - 35.f, m_canvas_size.y * 0.5f + 63.f);

    for (QuickSlot* slot : m_slot_vector)
        slot->removeFromParent();

    m_slot_vector.clear();

    const std::vector<ItemData>& item_vector = m_item_data_service->GetItems();
    for (int i = 0; i < item_vector.size(); i++)
    {
        QuickSlot* quick_slot = QuickSlot::create();
        quick_slot->Inject(item_vector[i], i);
        quick_slot->SetOnClickEvent(AX_CALLBACK_1(PlayUI::OnClickedSlot, this));

        Vec2 target_position = SLOT_START_POSITION;
        target_position.y -= i*(quick_slot->getContentSize().y + SLOT_SPACE);

        quick_slot->setPosition(target_position);
        this->addChild(quick_slot);

        m_slot_vector.push_back(quick_slot);
    }
}

void PlayUI::CreateDepthLabel()
{
    Vec2 depth_label_position = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5f - 250.f);
    m_depth_label             = Label::createWithTTF("[ 0m ]", "fonts/exqt.ttf", 20);
    m_depth_label->getFontAtlas()->setAliasTexParameters();
    m_depth_label->setPosition(depth_label_position);
    m_depth_label->setColor(Color32::GREEN);
    this->addChild(m_depth_label);
}



void PlayUI::UpdateUI(int depth)
{
    std::string target_string = fmt::format("[ {}m ]", -depth);
    m_depth_label->setString(target_string);
}



void PlayUI::OnClickedSlot(int slot_index)
{
    if (slot_index < 0 || slot_index >= m_slot_vector.size())
        return;

    QuickSlot* target_slot = m_slot_vector[slot_index];
    if (!target_slot)
        return;

    if (target_slot->IsCoolingDown())
        return;

    PlaySlotAction(slot_index);
    target_slot->StartCooldown();
}

void PlayUI::PlaySlotAction(int slot_index)
{
    ItemActionContext context;
    context.m_pickaxe   = m_pickaxe;
    context.m_chunck_manager = m_chunck_manager;
    context.m_layer_shaker   = m_layer_shaker;
    context.m_game_layer     = m_pickaxe->getParent();


    ItemCode item_code = m_slot_vector[slot_index]->GetItemCode();
    auto strategy      = ItemActionStrategyFactory::Create(item_code);
    if (strategy)
        strategy->Execute(context);

    m_inventory_service->DecreaseItem(item_code);
}
