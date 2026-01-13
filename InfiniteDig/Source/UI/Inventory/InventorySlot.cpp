#include "axmol/ui/UIScale9Sprite.h"
#include "InventorySlot.h"
#include "Data/Inventory/ItemSlotData.h"

using namespace ax::ui;
USING_NS_AX;

bool InventorySlot::init()
{
    if (!Sprite::init())
        return false;

    CreateUI();

    return true;
}

void InventorySlot::onEnter()
{
    Sprite::onEnter();

    m_update_listener = EventListenerCustom::create("INVENTORY_UPDATE", AX_CALLBACK_1(InventorySlot::UpdateUI, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_update_listener, this);
}

void InventorySlot::onExit()
{
    if (m_update_listener)
    {
        _eventDispatcher->removeEventListener(m_update_listener);
        m_update_listener = nullptr;
    }

    Sprite::onExit();
}

void InventorySlot::InitUI(ItemData item_data)
{
    m_item_code = item_data.m_item_code;

    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(item_data.m_frame_name);
    m_item_portrait_sprite->setSpriteFrame(frame);
}

void InventorySlot::CreateUI()
{
    m_canvas_size = _director->getCanvasSize();

    this->setCascadeOpacityEnabled(true);

    Vec2 slot_size = Vec2(32.f, 32.f);
    this->setContentSize(slot_size);

    m_item_portrait_sprite = Sprite::createWithSpriteFrameName("Coal Stuff.png");
    m_item_portrait_sprite->setCascadeOpacityEnabled(true);
    m_item_portrait_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_item_portrait_sprite->setPosition(slot_size * 0.5f);
    this->addChild(m_item_portrait_sprite);

    m_item_count_label = Label::createWithTTF("0", "fonts/exqt.ttf", 12);
    m_item_count_label->getFontAtlas()->setAliasTexParameters();
    m_item_count_label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    m_item_count_label->setPosition(Vec2(slot_size.x + 2.f, -6.f));
    m_item_count_label->setColor(Color32::WHITE);
    m_item_portrait_sprite->addChild(m_item_count_label);
}

void InventorySlot::UpdateUI(EventCustom* event)
{
    ItemSlotData item_slot_data = *static_cast<ItemSlotData*>(event->getUserData());
    if (m_item_code != item_slot_data.m_item_code)
        return;

    std::string target_string = fmt::format("{}", item_slot_data.m_item_count);
    m_item_count_label->setString(target_string);
}
