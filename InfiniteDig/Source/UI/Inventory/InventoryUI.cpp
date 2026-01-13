#include "axmol/ui/UIScale9Sprite.h"
#include "axmol/ui/UIButton.h"
#include "axmol/ui/UIScrollView.h"
#include "InventoryUI.h"
#include "InventorySlot.h"
#include "Utils/Manager/SoundManager.h"

using namespace ax::ui;
USING_NS_AX;

InventoryUI* InventoryUI::create(std::shared_ptr<Service::IInventoryService> inventory_service,
                                 std::shared_ptr<Service::IItemDataService> item_data_service)
{
    InventoryUI* inventory_ui = new (std::nothrow) InventoryUI();
    if (inventory_ui && inventory_ui->init(inventory_service, item_data_service))
    {
        inventory_ui->autorelease();
        return inventory_ui;
    }

    AX_SAFE_DELETE(inventory_ui);
    return nullptr;
}

bool InventoryUI::init(std::shared_ptr<Service::IInventoryService> inventory_service,
                       std::shared_ptr<Service::IItemDataService> item_data_service)
{
    if (!Layer::init())
        return false;

    m_inventory_service = inventory_service;
    m_item_data_service = item_data_service;

    CreateUI();

    return true;
}



void InventoryUI::OpenUI()
{
    ToggleUI(true);
    SetSelectToggle(m_stuff_toggle);
}

void InventoryUI::CloseUI()
{
    ToggleUI(false);
}

void InventoryUI::ToggleUI(bool active)
{
    const float TARGET_TIME = 0.25f;

    if (active)
    {
        CallFunc* enable_action = CallFunc::create([this]() { this->setVisible(true); });
        FadeIn* fade_in_action  = FadeIn::create(TARGET_TIME);
        Sequence* sequence_action = Sequence::create(enable_action, fade_in_action, nullptr);

        this->runAction(sequence_action);
    }
    else
    {
        FadeOut* fade_out_action = FadeOut::create(TARGET_TIME);
        CallFunc* disable_action = CallFunc::create([this]() { this->setVisible(false); });
        Sequence* sequence_action = Sequence::create(fade_out_action, disable_action, nullptr);

        this->runAction(sequence_action);
    }
}



void InventoryUI::CreateUI()
{
    this->setCascadeOpacityEnabled(true);
    this->setOpacity(0);
    this->setVisible(false);

    m_canvas_size = _director->getCanvasSize();

    CreateInventoryPanel();
    CreateTitleGroup();
    CreateToggleGroup();
    CreateStuffInventory();
    CreateRefineInventory();
    CreateToolInventory();
    CreateInventorySlots();
}

void InventoryUI::CreateInventoryPanel()
{
    Rect inventory_panel_center_rect = Rect(15.f, 15.f, 96.f - 15.f * 2, 96.f - 15.f * 2);
    Vec2 inventory_panel_size        = Vec2(224.f, 288.f);
    Vec2 inventory_panel_position    = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5f);

    m_inventory_panel = Scale9Sprite::createWithSpriteFrameName("Base Panel.png");
    m_inventory_panel->setCascadeOpacityEnabled(true);
    m_inventory_panel->setPosition(inventory_panel_position);
    m_inventory_panel->setContentSize(inventory_panel_size);
    this->addChild(m_inventory_panel);

    Scale9Sprite* inventory_panel = static_cast<Scale9Sprite*>(m_inventory_panel);
    inventory_panel->setCapInsets(inventory_panel_center_rect);

    m_panel_size = m_inventory_panel->getContentSize();

    Vec2 pattern_position      = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 28.f);
    Vec2 pattern_size          = Vec2(166.f, 16.f);
    Sprite* top_pattern_sprite = Sprite::createWithSpriteFrameName("Top Pattern.png");
    top_pattern_sprite->setPosition(pattern_position);
    top_pattern_sprite->setContentSize(pattern_size);
    m_inventory_panel->addChild(top_pattern_sprite);

    float dx[]{0, m_panel_size.x};
    float dy[]{0, m_panel_size.y};
    Rect knot_altas_rect = Rect(118.f, 75.f, 14.f, 14.f);

    float x_offset = 5.f;
    for (int x = 0; x < 2; x++)
    {
        float y_offset = 5.f;
        for (int y = 0; y < 2; y++)
        {
            Vec2 knot_position = Vec2(dx[x] + x_offset, dy[y] + y_offset);

            Sprite* knot_sprite = Sprite::createWithSpriteFrameName("Knot.png");
            knot_sprite->setPosition(knot_position);
            m_inventory_panel->addChild(knot_sprite);

            y_offset *= -1;
        }

        x_offset *= -1;
    }

    Rect inner_center_rect        = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Vec2 inventory_inner_size     = Vec2(184.f, 194.f);
    Vec2 inventory_inner_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f - 28.f);
    Scale9Sprite* inventory_inner = Scale9Sprite::createWithSpriteFrameName("Character Box.png");
    inventory_inner->setCapInsets(inventory_panel_center_rect);
    inventory_inner->setPosition(inventory_inner_position);
    inventory_inner->setContentSize(inventory_inner_size);
    m_inventory_panel->addChild(inventory_inner);
}

void InventoryUI::CreateTitleGroup()
{
    Rect name_panel_center_rect = Rect(4.f, 2.f, 64.f - 4.f * 2, 16.f - 2.f * 2);
    Vec2 name_panel_position    = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 4);
    Vec2 name_panel_size        = Vec2(96.f, 24.f);
    Scale9Sprite* name_panel    = Scale9Sprite::createWithSpriteFrameName("Title Box.png");
    name_panel->setCapInsets(name_panel_center_rect);
    name_panel->setCascadeOpacityEnabled(true);
    name_panel->setPosition(name_panel_position);
    name_panel->setContentSize(name_panel_size);
    m_inventory_panel->addChild(name_panel);

    const Vec2 NAME_PANEL_SIZE = name_panel->getContentSize();

    Vec2 name_label_position = Vec2(NAME_PANEL_SIZE.x * 0.5f, NAME_PANEL_SIZE.y * 0.5f);
    Color32 name_label_color = Color32(186, 145, 88);
    Label* name_label        = Label::createWithTTF("INVENTORY", "fonts/exqt.ttf", 16);
    name_label->setColor(name_label_color);
    name_label->setPosition(name_label_position);
    name_panel->addChild(name_label);
}

void InventoryUI::CreateToggleGroup()
{
    Sprite* stuff_on_sprite  = Sprite::createWithSpriteFrameName("Stuff On Toggle.png");
    Sprite* stuff_off_sprite = Sprite::createWithSpriteFrameName("Stuff Off Toggle.png");

    MenuItemSprite* stuff_on_menu_item  = MenuItemSprite::create(stuff_on_sprite, stuff_on_sprite);
    MenuItemSprite* stuff_off_menu_item = MenuItemSprite::create(stuff_off_sprite, stuff_off_sprite);

    m_stuff_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_0(InventoryUI::OnClickedStuff, this),
                                                        stuff_on_menu_item,
                                                        stuff_off_menu_item,
                                                        nullptr);
    m_stuff_toggle->setCascadeOpacityEnabled(true);

    Sprite* refine_on_sprite  = Sprite::createWithSpriteFrameName("Refine On Toggle.png");
    Sprite* refine_off_sprite = Sprite::createWithSpriteFrameName("Refine Off Toggle.png");

    MenuItemSprite* refine_on_menu_item  = MenuItemSprite::create(refine_on_sprite, refine_on_sprite);
    MenuItemSprite* refine_off_menu_item = MenuItemSprite::create(refine_off_sprite, refine_off_sprite);

    m_refine_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_0(InventoryUI::OnClickedRefine, this),
                                                         refine_on_menu_item,
                                                         refine_off_menu_item,
                                                         nullptr);
    m_refine_toggle->setCascadeOpacityEnabled(true);

    Sprite* tool_on_sprite  = Sprite::createWithSpriteFrameName("Item On Toggle.png");
    Sprite* tool_off_sprite = Sprite::createWithSpriteFrameName("Item Off Toggle.png");

    MenuItemSprite* tool_on_menu_item  = MenuItemSprite::create(tool_on_sprite, tool_on_sprite);
    MenuItemSprite* tool_off_menu_item = MenuItemSprite::create(tool_off_sprite, tool_off_sprite);

    m_tool_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_0(InventoryUI::OnClickedTool, this),
                                                       tool_on_menu_item,
                                                       tool_off_menu_item,
                                                       nullptr);
    m_tool_toggle->setCascadeOpacityEnabled(true);

    Vec2 toggle_menu_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 54.f);
    Menu* toggle_menu         = Menu::create(m_stuff_toggle, m_refine_toggle, m_tool_toggle, nullptr);
    toggle_menu->setCascadeOpacityEnabled(true);
    toggle_menu->setPosition(toggle_menu_position);
    toggle_menu->alignItemsHorizontallyWithPadding(16.f);
    m_inventory_panel->addChild(toggle_menu);
}

void InventoryUI::CreateStuffInventory()
{
    Vec2 inventory_layer_size     = Vec2(184.f, 194.f);
    Vec2 inventory_layer_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f);
    Vec2 inventory_layer_anchor   = Vec2::ANCHOR_MIDDLE;
    m_stuff_inventory_layer       = Sprite::create();
    m_stuff_inventory_layer->setCascadeOpacityEnabled(true);
    m_stuff_inventory_layer->setAnchorPoint(inventory_layer_anchor);
    m_stuff_inventory_layer->setPosition(inventory_layer_position);
    m_stuff_inventory_layer->setContentSize(inventory_layer_size);
    m_inventory_panel->addChild(m_stuff_inventory_layer);
}

void InventoryUI::CreateRefineInventory()
{
    Vec2 inventory_layer_size     = Vec2(184.f, 194.f);
    Vec2 inventory_layer_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f);
    Vec2 inventory_layer_anchor   = Vec2::ANCHOR_MIDDLE;
    m_refine_inventory_layer      = Sprite::create();
    m_refine_inventory_layer->setCascadeOpacityEnabled(true);
    m_refine_inventory_layer->setAnchorPoint(inventory_layer_anchor);
    m_refine_inventory_layer->setPosition(inventory_layer_position);
    m_refine_inventory_layer->setContentSize(inventory_layer_size);
    m_inventory_panel->addChild(m_refine_inventory_layer);
}

void InventoryUI::CreateToolInventory()
{
    Vec2 inventory_layer_size     = Vec2(184.f, 194.f);
    Vec2 inventory_layer_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f);
    Vec2 inventory_layer_anchor   = Vec2::ANCHOR_MIDDLE;
    m_tool_inventory_layer        = Sprite::create();
    m_tool_inventory_layer->setCascadeOpacityEnabled(true);
    m_tool_inventory_layer->setAnchorPoint(inventory_layer_anchor);
    m_tool_inventory_layer->setPosition(inventory_layer_position);
    m_tool_inventory_layer->setContentSize(inventory_layer_size);
    m_inventory_panel->addChild(m_tool_inventory_layer);
}

void InventoryUI::CreateInventorySlots()
{
    Vec2 inventory_layer_size = Vec2(184.f, 194.f);

    const int COLUMN      = 4;
    const float SLOT_WIDTH = 32.f;
    const float SLOT_HEIGHT = 38.f;
    const float PADDING_X = 12.f;
    const float PADDING_Y = 16.f;

    float grid_width = COLUMN * SLOT_WIDTH + (COLUMN - 1) * PADDING_X;
    float start_x    = (inventory_layer_size.x - grid_width) * 0.5f;
    float start_y    = inventory_layer_size.y - PADDING_Y - SLOT_HEIGHT;

    const std::vector<ItemData>& stuff_vector = m_item_data_service->GetStuffs();
    for (int i = 0; i < stuff_vector.size(); i++)
    {
        int col = i % COLUMN;
        int row = i / COLUMN;
        float x = start_x + col * (SLOT_WIDTH + PADDING_X) + SLOT_WIDTH * 0.5f;
        float y = start_y - row * (SLOT_HEIGHT + PADDING_Y);

        InventorySlot* slot = InventorySlot::create();
        slot->setPosition(Vec2(x, y));

        std::optional<ItemData> optional_item_data = m_item_data_service->GetItemData(stuff_vector[i].m_item_code);
        if (optional_item_data.has_value())
        {
            slot->InitUI(optional_item_data.value());
            m_stuff_inventory_layer->addChild(slot);
        }
    }

    const std::vector<ItemData>& refine_vector = m_item_data_service->GetRefines();
    for (int i = 0; i < refine_vector.size(); i++)
    {
        int col = i % COLUMN;
        int row = i / COLUMN;
        float x = start_x + col * (SLOT_WIDTH + PADDING_X) + SLOT_WIDTH * 0.5f;
        float y = start_y - row * (SLOT_HEIGHT + PADDING_Y);

        InventorySlot* slot = InventorySlot::create();
        slot->setPosition(Vec2(x, y));

        std::optional<ItemData> optional_item_data = m_item_data_service->GetItemData(refine_vector[i].m_item_code);
        if (optional_item_data.has_value())
        {
            slot->InitUI(optional_item_data.value());
            m_refine_inventory_layer->addChild(slot);
        }
    }

    const std::vector<ItemData>& item_vector = m_item_data_service->GetItems();
    for (int i = 0; i < item_vector.size(); i++)
    {
        int col = i % COLUMN;
        int row = i / COLUMN;
        float x = start_x + col * (SLOT_WIDTH + PADDING_X) + SLOT_WIDTH * 0.5f;
        float y = start_y - row * (SLOT_HEIGHT + PADDING_Y);

        InventorySlot* slot = InventorySlot::create();
        slot->setPosition(Vec2(x, y));

        std::optional<ItemData> optional_item_data = m_item_data_service->GetItemData(item_vector[i].m_item_code);
        if (optional_item_data.has_value())
        {
            slot->InitUI(optional_item_data.value());
            m_tool_inventory_layer->addChild(slot);
        }
    }
}



void InventoryUI::OnClickedStuff()
{
    SetSelectToggle(m_stuff_toggle);
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
}

void InventoryUI::OnClickedRefine()
{
    SetSelectToggle(m_refine_toggle);
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
}

void InventoryUI::OnClickedTool()
{
    SetSelectToggle(m_tool_toggle);
    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
}

void InventoryUI::SetSelectToggle(ax::MenuItemToggle* target_toggle)
{
    m_stuff_toggle->setSelectedIndex(m_stuff_toggle == target_toggle ? 0 : 1);
    m_stuff_inventory_layer->setVisible(m_stuff_toggle == target_toggle);

    m_refine_toggle->setSelectedIndex(m_refine_toggle == target_toggle ? 0 : 1);
    m_refine_inventory_layer->setVisible(m_refine_toggle == target_toggle);

    m_tool_toggle->setSelectedIndex(m_tool_toggle == target_toggle ? 0 : 1);
    m_tool_inventory_layer->setVisible(m_tool_toggle == target_toggle);
}
