#include "axmol/ui/UIScale9Sprite.h"
#include "ShopSlot.h"
#include "Data/Inventory/ItemSlotData.h"
#include "Utils/Manager/SoundManager.h"

using namespace std;
using namespace ax::ui;
using namespace Service;
USING_NS_AX;

bool ShopSlot::init()
{
    if (!Sprite::init())
        return false;

    CreateUI();

    return true;
}

void ShopSlot::onEnter()
{
    Sprite::onEnter();

    m_update_listener = EventListenerCustom::create("INVENTORY_UPDATE", AX_CALLBACK_1(ShopSlot::UpdateUI, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_update_listener, this);
}

void ShopSlot::onExit()
{
    if (m_update_listener)
    {
        _eventDispatcher->removeEventListener(m_update_listener);
        m_update_listener = nullptr;
    }

    Sprite::onExit();
}



void ShopSlot::InitUI(shared_ptr<IInventoryService> inventory_service,
                      ShopData shop_data,
                      ItemData item_data)
{
    m_inventory_service = inventory_service;

    m_item_code = item_data.m_item_code;
    m_item_cost = shop_data.m_item_cost;

    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(item_data.m_frame_name);
    m_portrait_sprite->setSpriteFrame(frame);

    m_name_label->setString(shop_data.m_item_name);
    m_description_label->setString(shop_data.m_item_desc);

    std::string cost_string = fmt::format("{}", shop_data.m_item_cost);
    Label* title_label      = m_purchase_button->getTitleLabel();
    title_label->setString(cost_string);
}

void ShopSlot::UpdateUI(ax::EventCustom* event)
{
    ItemSlotData item_slot_data = *static_cast<ItemSlotData*>(event->getUserData());
    if (ItemCode::COAL_INGOT != item_slot_data.m_item_code)
        return;

    m_purchase_button->setEnabled(item_slot_data.m_item_count >= m_item_cost);
    m_purchase_button->setTitleColor(item_slot_data.m_item_count >= m_item_cost ? Color32::BLACK : Color32::RED);
}


void ShopSlot::CreateUI()
{
    this->setCascadeOpacityEnabled(true);

    CreateSlotPanel();
    CreatePortraitGroup();
    CreateDescriptionGroup();
    CreatePurchaseButton();
}

void ShopSlot::CreateSlotPanel()
{
    Rect slot_center_rect     = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Vec2 shop_slot_panel_size = Vec2(192.f, 48.f);
    m_slot_panel              = Scale9Sprite::createWithSpriteFrameName("Item Box.png");
    m_slot_panel->setCascadeOpacityEnabled(true);
    m_slot_panel->setContentSize(shop_slot_panel_size);
    this->addChild(m_slot_panel);

    Scale9Sprite* slot_panel = static_cast<Scale9Sprite*>(m_slot_panel);
    slot_panel->setCapInsets(slot_center_rect);

    m_panel_size = m_slot_panel->getContentSize();
}

void ShopSlot::CreatePortraitGroup()
{
    Rect frame_center_rect    = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Vec2 frame_panel_size     = Vec2(38.f, 38.f);
    Vec2 frame_panel_position = Vec2(24.f, m_panel_size.y * 0.5f);
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

void ShopSlot::CreateDescriptionGroup()
{
    Vec2 name_label_position = Vec2(50.f, m_panel_size.y - 12.f);
    Color32 name_label_color = Color32(89, 148, 104);
    m_name_label             = Label::createWithTTF("아이템 이름", "fonts/exqt.ttf", 12);
    m_name_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_name_label->setPosition(name_label_position);
    m_name_label->setColor(name_label_color);
    m_name_label->setHorizontalAlignment(TextHAlignment::LEFT);
    m_slot_panel->addChild(m_name_label);

    Vec2 description_label_position = Vec2(50.f, m_panel_size.y - 32.f);
    Color32 description_label_color = Color32(186, 145, 88);
    m_description_label             = Label::createWithTTF("아이템 설명입니다.", "fonts/exqt.ttf", 12);
    m_description_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    m_description_label->setPosition(description_label_position);
    m_description_label->setColor(description_label_color);
    m_description_label->setHorizontalAlignment(TextHAlignment::LEFT);
    m_slot_panel->addChild(m_description_label);
}

void ShopSlot::CreatePurchaseButton()
{
    Vec2 purchase_button_position = Vec2(m_panel_size.x - 24.f, 16.f);
    Vec2 purchase_button_size     = Vec2(40.f, 20.f);
    m_purchase_button             = Button::create();
    m_purchase_button->loadTextureNormal("Purchase Button.png", Widget::TextureResType::PLIST);
    m_purchase_button->setCascadeOpacityEnabled(true);
    m_purchase_button->setScale9Enabled(true);
    m_purchase_button->setPosition(purchase_button_position);
    m_purchase_button->setContentSize(purchase_button_size);
    m_purchase_button->setTitleText("20");
    m_purchase_button->setTitleFontName("fonts/exqt.ttf");
    m_purchase_button->setTitleFontSize(12);
    m_purchase_button->setTitleColor(Color32::BLACK);
    m_purchase_button->setTitleAlignment(TextHAlignment::RIGHT);
    m_purchase_button->addClickEventListener(AX_CALLBACK_0(ShopSlot::OnClickedPurchase, this));
    m_slot_panel->addChild(m_purchase_button);

    const Vec2 BUTTON_SIZE = m_purchase_button->getContentSize();

    Vec2 title_label_position = Vec2(BUTTON_SIZE.x - 14.f, BUTTON_SIZE.y * 0.5f - 1.f);
    Label* title_label = m_purchase_button->getTitleLabel();
    title_label->setPosition(title_label_position);
}



void ShopSlot::OnClickedPurchase()
{
    int item_count = m_inventory_service->GetItemCount(ItemCode::COAL_INGOT);
    if (item_count < m_item_cost)
        return;

    m_inventory_service->DecreaseItem(ItemCode::COAL_INGOT, m_item_cost);
    m_inventory_service->IncreaseItem(m_item_code);

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Purchase.mp3");
}
