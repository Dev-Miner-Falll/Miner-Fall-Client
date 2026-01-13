#include "QuickSlot.h"
#include "Data/Inventory/ItemSlotData.h"
#include "Utils/Manager/SoundManager.h"

USING_NS_AX;

bool QuickSlot::init()
{
    if (!Sprite::init())
        return false;

    CreateUI();

    return true;
}

void QuickSlot::onEnter()
{
    Sprite::onEnter();

    if (m_touch_listener)
        return;

    m_touch_listener = EventListenerTouchOneByOne::create();
    m_touch_listener->setSwallowTouches(true);

    m_touch_listener->onTouchBegan = [this](Touch* touch, Event*)
    {
        Vec2 local_position = this->convertToNodeSpace(touch->getLocation());
        Rect slot_rect      = Rect(Vec2::ZERO, m_content_size);

        if (slot_rect.containsPoint(local_position))
        {
            if (m_is_cooling || !m_is_active)
            {
                this->stopAllActions();

                ScaleTo* scale_start_action = ScaleTo::create(0.05f, 0.95f);
                ScaleTo* scale_end_action   = ScaleTo::create(0.05f, 1.0f);
                Sequence* sequence_action = Sequence::create(scale_start_action, scale_end_action, nullptr);
                this->runAction(sequence_action);

                SoundManager::getInstance()->PlaySFX("Sounds/SFX/Disabled Button.mp3");
            }
            else if (m_on_click)
            {
                m_on_click(m_slot_index);
            }

            return true;
        }

        return false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touch_listener, this);

    if (m_update_listener)
        return;

    m_update_listener = EventListenerCustom::create("INVENTORY_UPDATE", AX_CALLBACK_1(QuickSlot::UpdateUI, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_update_listener, this);
}

void QuickSlot::onExit()
{
    if (m_touch_listener)
    {
        _eventDispatcher->removeEventListener(m_touch_listener);
        m_touch_listener = nullptr;
    }

    if (m_update_listener)
    {
        _eventDispatcher->removeEventListener(m_update_listener);
        m_update_listener = nullptr;
    }

    Sprite::onExit();
}

void QuickSlot::Inject(ItemData item_data, int slot_index)
{
    m_item_data  = item_data;
    m_slot_index = slot_index;

    m_left_cooltime = 0.f;
    m_is_cooling    = false;

    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(item_data.m_frame_name);
    m_icon_sprite->setSpriteFrame(frame);

    m_cool_mask->setPercentage(0.f);
}

void QuickSlot::SetOnClickEvent(std::function<void(int)> callback)
{
    m_on_click = callback;
}



void QuickSlot::update(float delta)
{
    if (!m_is_cooling)
        return;

    m_left_cooltime -= delta;
    if (m_left_cooltime <= 0.f)
    {
        m_left_cooltime = 0.f;
        m_is_cooling    = false;
        m_cool_mask->setPercentage(0.f);

        ScaleTo* scale_start_action = ScaleTo::create(0.1f, 1.1f);
        ScaleTo* scale_end_action = ScaleTo::create(0.1f, 1.0f);
        Sequence* sequence_action   = Sequence::create(scale_start_action, scale_end_action, nullptr);
        this->runAction(sequence_action);

        unscheduleUpdate();
        return;
    }

    UpdateCoolingMask();
}

void QuickSlot::UpdateCoolingMask()
{
    float progress = m_left_cooltime / m_item_data.m_cooltime;
    float ratio    = progress * 100.f;

    m_cool_mask->setPercentage(ratio);
}

void QuickSlot::UpdateUI(ax::EventCustom* event)
{
    ItemSlotData item_slot_data = *static_cast<ItemSlotData*>(event->getUserData());
    if (m_item_data.m_item_code != item_slot_data.m_item_code)
        return;

    m_is_active = item_slot_data.m_item_count > 0;

    std::string target_string = fmt::format("{}", item_slot_data.m_item_count);
    m_count_label->setString(target_string);
    m_count_label->setColor(m_is_active ? Color32::WHITE : Color32::RED);
}



void QuickSlot::StartCooldown()
{
    m_is_cooling = true;
    m_left_cooltime = m_item_data.m_cooltime;
    m_cool_mask->setPercentage(100.f);

    scheduleUpdate();
}

bool QuickSlot::IsCoolingDown() const
{
    return m_is_cooling;
}



void QuickSlot::CreateUI()
{
    m_content_size = Vec2(41.f, 41.f);
    this->setContentSize(m_content_size);
    this->setCascadeOpacityEnabled(true);

    CreateFrame();
    CreateMask();
    CreateCountLabel();
}

void QuickSlot::CreateFrame()
{
    Vec2 frame_position   = Vec2(m_content_size.x * 0.5f, m_content_size.y * 0.5f);
    Sprite* frame_sprite = Sprite::createWithSpriteFrameName("Quick Slot.png");
    frame_sprite->setPosition(frame_position);
    this->addChild(frame_sprite);

    Vec2 icon_size     = Vec2(32.f, 32.f);
    Vec2 icon_position = Vec2(m_content_size.x * 0.5f, m_content_size.y * 0.5f);
    m_icon_sprite = Sprite::create();
    m_icon_sprite->setCascadeOpacityEnabled(true);
    m_icon_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_icon_sprite->setPosition(icon_position);
    m_icon_sprite->setContentSize(icon_size);
    this->addChild(m_icon_sprite);
}

void QuickSlot::CreateMask()
{
    Sprite* timer_sprite = Sprite::createWithSpriteFrameName("Quick Slot.png");

    m_cool_mask = ProgressTimer::create(timer_sprite);
    m_cool_mask->setCascadeOpacityEnabled(true);
    m_cool_mask->setOpacity(240);
    m_cool_mask->setType(ProgressTimer::Type::RADIAL);
    m_cool_mask->setReverseDirection(true);
    m_cool_mask->setPercentage(0.f);
    m_cool_mask->setPosition(m_icon_sprite->getPosition());
    this->addChild(m_cool_mask);
}

void QuickSlot::CreateCountLabel()
{
    Vec2 label_frame_positon = Vec2(m_content_size.x - 5, 5);
    Sprite* frame_sprite     = Sprite::createWithSpriteFrameName("Quick Count.png");
    frame_sprite->setCascadeOpacityEnabled(true);
    frame_sprite->setPosition(label_frame_positon);
    this->addChild(frame_sprite);

    const Vec2 FRAME_SIZE = frame_sprite->getContentSize();

    Vec2 label_position = Vec2(FRAME_SIZE.x * 0.5f + 1.f, FRAME_SIZE.y * 0.5f - 1.f);
    m_count_label       = Label::createWithTTF("", "fonts/exqt.ttf", 12);
    m_count_label->setPosition(label_position);
    frame_sprite->addChild(m_count_label);
}
