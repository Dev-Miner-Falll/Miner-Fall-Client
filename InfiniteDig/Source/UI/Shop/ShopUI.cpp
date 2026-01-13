#include <iostream>
#include "axmol/ui/UIScale9Sprite.h"
#include "axmol/ui/UIScrollView.h"
#include "ShopUI.h"
#include "ShopSlot.h"

using namespace ax::ui;
USING_NS_AX;

ShopUI* ShopUI::create(std::shared_ptr<Service::IInventoryService> inventory_service,
                       std::shared_ptr<Service::IItemDataService> item_data_service,
                       std::shared_ptr<Service::IShopDataService> shop_data_service)
{
    ShopUI* shop_ui = new (std::nothrow) ShopUI();
    if (shop_ui && shop_ui->init(inventory_service, item_data_service, shop_data_service))
    {
        shop_ui->autorelease();
        return shop_ui;
    }

    AX_SAFE_DELETE(shop_ui);
    return nullptr;
}

bool ShopUI::init(std::shared_ptr<Service::IInventoryService> inventory_service,
                  std::shared_ptr<Service::IItemDataService> item_data_service,
                  std::shared_ptr<Service::IShopDataService> shop_data_service)
{
    if (!Layer::init())
        return false;

    m_inventory_service = inventory_service;
    m_item_data_service = item_data_service;
    m_shop_data_service = shop_data_service;

    CreateUI();

    return true;
}



void ShopUI::OpenUI()
{
    ToggleUI(true);
}

void ShopUI::CloseUI()
{
    ToggleUI(false);
}

void ShopUI::ToggleUI(bool active)
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



void ShopUI::CreateUI()
{
    this->setCascadeOpacityEnabled(true);
    this->setOpacity(0);
    this->setVisible(false);

    m_canvas_size = _director->getCanvasSize();

    CreateShopPanel();
    CreateTitleGroup();
    CreateScrollView();
    CreateShopSlots();
}

void ShopUI::CreateShopPanel()
{
    Rect shop_panel_center_rect = Rect(15.f, 15.f, 96.f - 15.f * 2, 96.f - 15.f * 2);
    Vec2 shop_panel_size        = Vec2(224.f, 288.f);
    Vec2 shop_panel_position    = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5f);
    m_shop_panel = Scale9Sprite::createWithSpriteFrameName("Base Panel.png");
    m_shop_panel->setCascadeOpacityEnabled(true);
    m_shop_panel->setPosition(shop_panel_position);
    m_shop_panel->setContentSize(shop_panel_size);
    this->addChild(m_shop_panel);

    Scale9Sprite* shop_panel = static_cast<Scale9Sprite*>(m_shop_panel);
    shop_panel->setCapInsets(shop_panel_center_rect);

    m_panel_size = m_shop_panel->getContentSize();

    Vec2 pattern_position      = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 28.f);
    Vec2 pattern_size          = Vec2(166.f, 16.f);
    Sprite* top_pattern_sprite = Sprite::createWithSpriteFrameName("Top Pattern.png");
    top_pattern_sprite->setPosition(pattern_position);
    top_pattern_sprite->setContentSize(pattern_size);
    m_shop_panel->addChild(top_pattern_sprite);

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
            m_shop_panel->addChild(knot_sprite);

            y_offset *= -1;
        }

        x_offset *= -1;
    }
}

void ShopUI::CreateTitleGroup()
{
    Rect name_panel_center_rect = Rect(4.f, 2.f, 64.f - 4.f * 2, 16.f - 2.f * 2);
    Vec2 name_panel_position    = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 4);
    Vec2 name_panel_size        = Vec2(96.f, 24.f);
    Scale9Sprite* name_panel    = Scale9Sprite::createWithSpriteFrameName("Title Box.png");
    name_panel->setCapInsets(name_panel_center_rect);
    name_panel->setCascadeOpacityEnabled(true);
    name_panel->setPosition(name_panel_position);
    name_panel->setContentSize(name_panel_size);
    m_shop_panel->addChild(name_panel);

    const Vec2 NAME_PANEL_SIZE = name_panel->getContentSize();

    Vec2 name_label_position = Vec2(NAME_PANEL_SIZE.x * 0.5f, NAME_PANEL_SIZE.y * 0.5f);
    Color32 name_label_color = Color32(186, 145, 88);
    Label* name_label        = Label::createWithTTF("SHOP", "fonts/exqt.ttf", 16);
    name_label->setColor(name_label_color);
    name_label->setPosition(name_label_position);
    name_panel->addChild(name_label);
}

void ShopUI::CreateScrollView()
{
    Vec2 scroll_view_size     = Vec2(204.f, 224.f);
    Vec2 scroll_view_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f - 10);
    m_scroll_view             = ScrollView::create();

    ScrollView* scroll_view = static_cast<ScrollView*>(m_scroll_view);
    scroll_view->setDirection(ScrollView::Direction::VERTICAL);
    scroll_view->setBounceEnabled(false);
    scroll_view->setScrollBarEnabled(false);
    scroll_view->setContentSize(scroll_view_size);
    scroll_view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    scroll_view->setPosition(scroll_view_position);
    m_shop_panel->addChild(m_scroll_view);
}

void ShopUI::CreateShopSlots()
{
    ScrollView* scroll_view    = static_cast<ScrollView*>(m_scroll_view);

    const std::vector<ShopData>& shop_data_vector = m_shop_data_service->GetAllShopDatas();

    const int SLOT_COUNT       = m_shop_data_service->GetAllShopDatas().size();
    const float SLOT_HEIGHT    = 48.0f;
    const float SLOT_SPACE     = 6.0f;
    const float CONTENT_HEIGHT = SLOT_COUNT * SLOT_HEIGHT + (SLOT_COUNT + 1) * SLOT_SPACE;
    const float VIEW_HEIGHT    = scroll_view->getContentSize().height;

    float content_height = std::max(CONTENT_HEIGHT, VIEW_HEIGHT);

    
    Size container_size = Size(216.f, content_height);
    scroll_view->setInnerContainerSize(container_size);

    Layout* inner = scroll_view->getInnerContainer();

    const float START_Y = content_height - SLOT_SPACE - SLOT_HEIGHT * 0.5f;

    for (int i = 0; i < shop_data_vector.size(); i++)
    {
        std::optional<ShopData> optional_shop_data = m_shop_data_service->GetShopData(shop_data_vector[i].m_item_code);
        if (!optional_shop_data.has_value())
            continue;

        std::optional<ItemData> optional_item_data = m_item_data_service->GetItemData(shop_data_vector[i].m_item_code);
        if (!optional_item_data.has_value())
            continue;

        ShopSlot* shop_slot = ShopSlot::create();
        shop_slot->InitUI(m_inventory_service, optional_shop_data.value(), optional_item_data.value());

        float x = container_size.width * 0.5f - 6;
        float y = START_Y - i * (SLOT_HEIGHT + SLOT_SPACE);

        shop_slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        shop_slot->setPosition(Vec2(x, y));

        inner->addChild(shop_slot);
    }
}
