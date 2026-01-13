#include "IngredientSlot.h"
#include "Data/Inventory/ItemSlotData.h"

using namespace ax::ui;
USING_NS_AX;

bool IngredientSlot::init()
{
    if (!Sprite::init())
        return false;

    CreateUI();

    return true;
}

void IngredientSlot::onEnter()
{
    Sprite::onEnter();

    m_update_listener = EventListenerCustom::create("INVENTORY_UPDATE", AX_CALLBACK_1(IngredientSlot::UpdateUI, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_update_listener, this);
}

void IngredientSlot::onExit()
{
    if (m_update_listener)
    {
        _eventDispatcher->removeEventListener(m_update_listener);
        m_update_listener = nullptr;
    }

    Sprite::onExit();
}

void IngredientSlot::InitUI(ItemData item_data,
                            IngredientData ingredient_data,
                            std::shared_ptr<Service::IInventoryService> inventory_service)
{
    m_item_data = item_data;
    m_ingredient_data = ingredient_data;
    m_inventory_service = inventory_service;

    m_portrait_sprite->setSpriteFrame(item_data.m_frame_name);
    m_name_label->setString(item_data.m_item_name);
    m_count_label->setString(fmt::format("x {}", ingredient_data.m_ingredient_count));

    bool can_craftable = m_inventory_service->GetItemCount(item_data.m_item_code) >= ingredient_data.m_ingredient_count;
    m_count_label->setColor(can_craftable ? Color32::WHITE : Color32::RED);
}


void IngredientSlot::OnEnable() {}

void IngredientSlot::OnDisable() {}

ObjectType IngredientSlot::GetObjectType() const
{
    return ObjectType::INGREDIENT_SLOT;
}



void IngredientSlot::CreateUI()
{
    this->setCascadeOpacityEnabled(true);

    CreateSlotPanel();
    CreatePortrait();
    CreateDescriptionLabel();
}

void IngredientSlot::CreateSlotPanel()
{
    Rect slot_center_rect = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Vec2 slot_panel_size  = Vec2(172.f, 48.f);
    m_slot_panel          = Scale9Sprite::createWithSpriteFrameName("Item Box.png");
    m_slot_panel->setCascadeOpacityEnabled(true);
    m_slot_panel->setContentSize(slot_panel_size);
    m_slot_panel->setCapInsets(slot_center_rect);
    m_slot_panel->setColor(Color32(230, 230, 230));
    this->addChild(m_slot_panel);

    m_slot_size = m_slot_panel->getContentSize();
}

void IngredientSlot::CreatePortrait()
{
    Rect frame_center_rect    = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Vec2 frame_panel_size     = Vec2(38.f, 38.f);
    Vec2 frame_panel_position = Vec2(24.f, m_slot_size.y * 0.5f);
    Color32 frame_panel_color = Color32(230, 230, 230);
    Scale9Sprite* frame_panel = Scale9Sprite::createWithSpriteFrameName("Item Box.png");
    frame_panel->setCapInsets(frame_center_rect);
    frame_panel->setCascadeOpacityEnabled(true);
    frame_panel->setPosition(frame_panel_position);
    frame_panel->setContentSize(frame_panel_size);
    frame_panel->setColor(frame_panel_color);
    m_slot_panel->addChild(frame_panel);

    const Vec2 FRAME_SIZE = frame_panel->getContentSize();

    Vec2 portrait_size     = Vec2(32.f, 32.f);
    Vec2 portrait_position = Vec2(FRAME_SIZE.x * 0.5f, FRAME_SIZE.y * 0.5f);
    m_portrait_sprite      = Sprite::create();
    m_portrait_sprite->setPosition(portrait_position);
    m_portrait_sprite->setContentSize(portrait_size);
    frame_panel->addChild(m_portrait_sprite);
}

void IngredientSlot::CreateDescriptionLabel()
{
    Vec2 name_label_position = Vec2(50.f, m_slot_size.y - 12.f);
    Color32 name_label_color = Color32(89, 148, 104);
    m_name_label             = Label::createWithTTF("아이템 이름", "fonts/exqt.ttf", 12);
    m_name_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_name_label->setPosition(name_label_position);
    m_name_label->setColor(name_label_color);
    m_name_label->setHorizontalAlignment(TextHAlignment::LEFT);
    m_slot_panel->addChild(m_name_label);

    Vec2 count_label_position = Vec2(50.f, m_slot_size.y - 25.f);
    m_count_label             = Label::createWithTTF("x 1", "fonts/exqt.ttf", 12);
    m_count_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_count_label->setPosition(count_label_position);
    m_count_label->setHorizontalAlignment(TextHAlignment::LEFT);
    m_slot_panel->addChild(m_count_label);
}

void IngredientSlot::UpdateUI(EventCustom* event)
{
    ItemSlotData item_slot_data = *static_cast<ItemSlotData*>(event->getUserData());
    if (m_item_data.m_item_code != item_slot_data.m_item_code)
        return;

    bool can_craftable = m_ingredient_data.m_ingredient_count <= item_slot_data.m_item_count;
    m_count_label->setColor(can_craftable ? Color32::WHITE : Color32::RED);
}
