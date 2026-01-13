#include "ToggleUI.h"
#include "axmol/ui/UIButton.h"
#include "Utils/Manager/SoundManager.h"

using namespace ax::ui;
USING_NS_AX;

ToggleUI* ToggleUI::create(IOpenableUI* play_ui,
                           IOpenableUI* inventory_ui,
                           IOpenableUI* shop_ui,
                           IOpenableUI* craft_ui,
                           IOpenableUI* setting_ui,
                           ax::Layer* setting_layer)
{
    ToggleUI* toggle_ui = new (std::nothrow) ToggleUI();
    if (toggle_ui && toggle_ui->init(play_ui, inventory_ui, shop_ui, setting_ui, craft_ui, setting_layer))
    {
        toggle_ui->autorelease();
        return toggle_ui;
    }

    AX_SAFE_DELETE(toggle_ui);
    return nullptr;
}

bool ToggleUI::init(IOpenableUI* play_ui,
                    IOpenableUI* inventory_ui,
                    IOpenableUI* shop_ui,
                    IOpenableUI* craft_ui,
                    IOpenableUI* setting_ui,
                    ax::Layer* setting_layer)
{
    if (!Layer::init())
        return false;

    m_setting_layer = setting_layer;
    m_play_ui      = play_ui;
    m_inventory_ui = inventory_ui;
    m_shop_ui      = shop_ui;
    m_craft_ui      = craft_ui;
    m_setting_ui    = setting_ui;

    CreateUI();
    SetSelectedToggle(m_dig_toggle);

    return true;
}



void ToggleUI::CreateUI()
{
    m_canvas_size = _director->getCanvasSize();

    CreateDigToggle();
    CreateInventoryToggle();
    CreateShopToggle();
    CreateCraftToggle();
    CreateToggleMenu();

    CreateSettingButton();
}

void ToggleUI::CreateToggleMenu()
{
    Vec2 toggle_menu_position = Vec2(m_canvas_size.x * 0.5f, 25);
    Menu* toggle_menu         = Menu::create(m_dig_toggle, m_inventory_toggle, m_shop_toggle, m_craft_toggle, nullptr);
    toggle_menu->setPosition(toggle_menu_position);
    toggle_menu->alignItemsHorizontallyWithPadding(16.f);
    this->addChild(toggle_menu);
}

void ToggleUI::CreateDigToggle()
{
    Color32 toggle_on_color  = Color32::WHITE;
    Color32 toggle_off_color = Color32::GRAY;

    Sprite* dig_on_sprite = Sprite::createWithSpriteFrameName("Play Icon.png");
    dig_on_sprite->setColor(toggle_on_color);

    Sprite* dig_off_sprite = Sprite::createWithSpriteFrameName("Play Icon.png");
    dig_off_sprite->setColor(toggle_off_color);

    MenuItemSprite* dig_on_menu_item  = MenuItemSprite::create(dig_on_sprite, dig_on_sprite);
    MenuItemSprite* dig_off_menu_item = MenuItemSprite::create(dig_off_sprite, dig_off_sprite);

    m_dig_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_0(ToggleUI::OnClickedDigToggle, this),
                                                      dig_on_menu_item,
                                                      dig_off_menu_item,
                                                      nullptr);
}

void ToggleUI::CreateShopToggle()
{
    Color32 toggle_on_color  = Color32::WHITE;
    Color32 toggle_off_color = Color32::GRAY;

    Sprite* shop_on_sprite = Sprite::createWithSpriteFrameName("Shop Icon.png");
    shop_on_sprite->setColor(toggle_on_color);

    Sprite* shop_off_sprite = Sprite::createWithSpriteFrameName("Shop Icon.png");
    shop_off_sprite->setColor(toggle_off_color);

    MenuItemSprite* shop_on_menu_item  = MenuItemSprite::create(shop_on_sprite, shop_on_sprite);
    MenuItemSprite* shop_off_menu_item = MenuItemSprite::create(shop_off_sprite, shop_off_sprite);

    m_shop_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_0(ToggleUI::OnClickedShopToggle, this),
                                                       shop_on_menu_item,
                                                       shop_off_menu_item,
                                                       nullptr);
}

void ToggleUI::CreateCraftToggle()
{
    Color32 toggle_on_color  = Color32::WHITE;
    Color32 toggle_off_color = Color32::GRAY;

    Sprite* craft_on_sprite = Sprite::createWithSpriteFrameName("Forge Icon.png");
    craft_on_sprite->setColor(toggle_on_color);

    Sprite* craft_off_sprite = Sprite::createWithSpriteFrameName("Forge Icon.png");
    craft_off_sprite->setColor(toggle_off_color);

    MenuItemSprite* craft_on_menu_item  = MenuItemSprite::create(craft_on_sprite, craft_on_sprite);
    MenuItemSprite* craft_off_menu_item = MenuItemSprite::create(craft_off_sprite, craft_off_sprite);

    m_craft_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_0(ToggleUI::OnClickedCraftToggle, this),
                                                        craft_on_menu_item,
                                                        craft_off_menu_item,
                                                        nullptr);
}

void ToggleUI::CreateInventoryToggle()
{
    Color32 toggle_on_color    = Color32::WHITE;
    Color32 toggle_off_color   = Color32::GRAY;

    Sprite* inventory_on_sprite = Sprite::createWithSpriteFrameName("Inventory Icon.png");
    inventory_on_sprite->setColor(toggle_on_color);

    Sprite* inventory_off_sprite = Sprite::createWithSpriteFrameName("Inventory Icon.png");
    inventory_off_sprite->setColor(toggle_off_color);

    MenuItemSprite* craft_on_menu_item  = MenuItemSprite::create(inventory_on_sprite, inventory_on_sprite);
    MenuItemSprite* craft_off_menu_item = MenuItemSprite::create(inventory_off_sprite, inventory_off_sprite);

    m_inventory_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_0(ToggleUI::OnClickedInventoryToggle, this),
                                                            craft_on_menu_item,
                                                            craft_off_menu_item,
                                                            nullptr);
}

void ToggleUI::CreateSettingButton()
{
    Vec2 setting_button_position = Vec2(m_canvas_size.x - 30.f, m_canvas_size.y - 30.f);
    Button* setting_button = Button::create();
    setting_button->loadTextureNormal("Setting Icon.png", Widget::TextureResType::PLIST);
    setting_button->setPosition(setting_button_position);
    setting_button->addClickEventListener(AX_CALLBACK_0(ToggleUI::OnClickedSettingButton, this));
    m_setting_layer->addChild(setting_button);
}



void ToggleUI::OnClickedDigToggle()
{
    SetSelectedToggle(m_dig_toggle);
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Toggle Interaction2.mp3");
}

void ToggleUI::OnClickedInventoryToggle()
{
    SetSelectedToggle(m_inventory_toggle);
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Toggle Interaction2.mp3");
}

void ToggleUI::OnClickedShopToggle()
{
    SetSelectedToggle(m_shop_toggle);
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Toggle Interaction2.mp3");
}

void ToggleUI::OnClickedCraftToggle()
{
    SetSelectedToggle(m_craft_toggle);
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Toggle Interaction2.mp3");
}

void ToggleUI::OnClickedSettingButton()
{
    m_setting_ui->OpenUI();
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Toggle Interaction2.mp3");
}

void ToggleUI::SetSelectedToggle(ax::MenuItemToggle* target_toggle)
{
    m_dig_toggle->setSelectedIndex(m_dig_toggle == target_toggle ? 0 : 1);
    if (m_play_ui)
    {
        if (m_dig_toggle == target_toggle)
            m_play_ui->OpenUI();
        else
            m_play_ui->CloseUI();
    }

    m_inventory_toggle->setSelectedIndex(m_inventory_toggle == target_toggle ? 0 : 1);
    if (m_inventory_ui)
    {
        if (m_inventory_toggle == target_toggle)
            m_inventory_ui->OpenUI();
        else
            m_inventory_ui->CloseUI();
    }

    m_shop_toggle->setSelectedIndex(m_shop_toggle == target_toggle ? 0 : 1);
    if (m_shop_ui)
    {
        if (m_shop_toggle == target_toggle)
            m_shop_ui->OpenUI();
        else
            m_shop_ui->CloseUI();
    }

    m_craft_toggle->setSelectedIndex(m_craft_toggle == target_toggle ? 0 : 1);
    if (m_craft_ui)
    {
        if (m_craft_toggle == target_toggle)
            m_craft_ui->OpenUI();
        else
            m_craft_ui->CloseUI();
    }
}
