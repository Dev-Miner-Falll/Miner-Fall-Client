#include "RecipeSlot.h"

using namespace ax::ui;
USING_NS_AX;

bool RecipeSlot::init()
{
    if (!Sprite::init())
        return false;

    CreateUI();

    return true;
}



void RecipeSlot::InitUI(ItemData item_data, std::function<void(ItemCode)> click_callback)
{
    m_item_data = item_data;
    m_click_callback = click_callback;

    m_portrait_sprite->setSpriteFrame(item_data.m_frame_name);
    m_name_label->setString(item_data.m_item_name);
}



void RecipeSlot::CreateUI()
{
    this->setCascadeOpacityEnabled(true);

    CreateSlotPanel();
    CreatePortrait();
    CreateNameLabel();
    CreateCraftButton();
}

void RecipeSlot::CreateSlotPanel()
{
    Rect slot_center_rect     = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Vec2 slot_panel_size = Vec2(192.f, 48.f);
    m_slot_panel              = Scale9Sprite::createWithSpriteFrameName("Item Box.png");
    m_slot_panel->setCascadeOpacityEnabled(true);
    m_slot_panel->setContentSize(slot_panel_size);
    m_slot_panel->setCapInsets(slot_center_rect);
    this->addChild(m_slot_panel);

    m_slot_size = m_slot_panel->getContentSize();
}

void RecipeSlot::CreatePortrait()
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

void RecipeSlot::CreateNameLabel()
{
    Vec2 name_label_position = Vec2(50.f, m_slot_size.y - 12.f);
    Color32 name_label_color = Color32(186, 145, 88);
    m_name_label             = Label::createWithTTF("아이템 이름", "fonts/exqt.ttf", 12);
    m_name_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_name_label->setPosition(name_label_position);
    m_name_label->setColor(name_label_color);
    m_name_label->setHorizontalAlignment(TextHAlignment::LEFT);
    m_slot_panel->addChild(m_name_label);
}

void RecipeSlot::CreateCraftButton()
{
    Vec2 button_position = Vec2(m_slot_size.x - 32, 12);
    m_craft_button       = Button::create();
    m_craft_button->setCascadeOpacityEnabled(true);
    m_craft_button->loadTextureNormal("Button.png", Widget::TextureResType::PLIST);
    m_craft_button->setPosition(button_position);
    m_craft_button->setTitleText("레시피");
    m_craft_button->setTitleColor(Color32::BLACK);
    m_craft_button->setTitleFontName("fonts/exqt.ttf");
    m_craft_button->setTitleFontSize(12);
    m_craft_button->addClickEventListener(AX_CALLBACK_0(RecipeSlot::OnClickedButton, this));
    m_slot_panel->addChild(m_craft_button);
}



void RecipeSlot::OnClickedButton()
{
    if (m_click_callback)
        m_click_callback(m_item_data.m_item_code);
}
