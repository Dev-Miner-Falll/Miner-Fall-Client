#include "CraftUI.h"
#include "axmol/ui/UIScale9Sprite.h"
#include "axmol/ui/UIScrollView.h"
#include "Utils/Pool/ObjectPoolManager.h"
#include "Utils/Manager/SoundManager.h"

using namespace ax::ui;
USING_NS_AX;

CraftUI* CraftUI::create(std::shared_ptr<Service::IRecipeDataService> recipe_data_service,
                         std::shared_ptr<Service::IItemDataService> item_data_service,
                         std::shared_ptr<Service::IInventoryService> inventory_service)
{
    CraftUI* craft_ui = new (std::nothrow) CraftUI();
    if (craft_ui && craft_ui->init(recipe_data_service, item_data_service, inventory_service))
    {
        craft_ui->autorelease();
        return craft_ui;
    }

    AX_SAFE_DELETE(craft_ui);
    return nullptr;
}

bool CraftUI::init(std::shared_ptr<Service::IRecipeDataService> recipe_data_service,
                   std::shared_ptr<Service::IItemDataService> item_data_service,
                   std::shared_ptr<Service::IInventoryService> inventory_service)
{
    if (!Layer::init())
        return false;

    m_recipe_data_service = recipe_data_service;
    m_item_data_service   = item_data_service;
    m_inventory_service   = inventory_service;

    CreateUI();

    return true;
}

void CraftUI::onEnter()
{
    m_update_listener = EventListenerCustom::create("INVENTORY_UPDATE", AX_CALLBACK_1(CraftUI::UpdateUI, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_update_listener, this);

    Layer::onEnter();
}

void CraftUI::onExit()
{
    if (m_update_listener)
    {
        _eventDispatcher->removeEventListener(m_update_listener);
        m_update_listener = nullptr;
    }

    Layer::onExit();
}



void CraftUI::OpenUI()
{
    ToggleUI(true);
    ToggleLayer(false, true);
}

void CraftUI::CloseUI()
{
    ToggleUI(false);
}

void CraftUI::ToggleUI(bool active)
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



void CraftUI::CreateUI()
{
    m_canvas_size = _director->getCanvasSize();

    this->setCascadeOpacityEnabled(true);
    this->setOpacity(0);
    this->setVisible(false);

    CreateCraftPanel();
    CreateTitleGroup();
    CreateRecipeLayer();
    CreateRecipeSlots();
    CreateCraftLayer();
}

void CraftUI::CreateCraftPanel()
{
    Rect craft_panel_center_rect = Rect(15.f, 15.f, 96.f - 15.f * 2, 96.f - 15.f * 2);
    Vec2 craft_panel_size        = Vec2(224.f, 288.f);
    Vec2 craft_panel_position    = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5f);
    Scale9Sprite* craft_panel    = Scale9Sprite::createWithSpriteFrameName("Base Panel.png");
    craft_panel->setCascadeOpacityEnabled(true);
    craft_panel->setPosition(craft_panel_position);
    craft_panel->setContentSize(craft_panel_size);
    craft_panel->setCapInsets(craft_panel_center_rect);
    this->addChild(craft_panel);

    m_craft_panel = craft_panel;
    m_panel_size = craft_panel->getContentSize();

    Vec2 pattern_position      = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 28.f);
    Vec2 pattern_size          = Vec2(166.f, 16.f);
    Sprite* top_pattern_sprite = Sprite::createWithSpriteFrameName("Top Pattern.png");
    top_pattern_sprite->setPosition(pattern_position);
    top_pattern_sprite->setContentSize(pattern_size);
    craft_panel->addChild(top_pattern_sprite);

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
            craft_panel->addChild(knot_sprite);

            y_offset *= -1;
        }

        x_offset *= -1;
    }
}

void CraftUI::CreateTitleGroup()
{
    Rect name_panel_center_rect = Rect(4.f, 2.f, 64.f - 4.f * 2, 16.f - 2.f * 2);
    Vec2 name_panel_position    = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 4);
    Vec2 name_panel_size        = Vec2(96.f, 24.f);
    Scale9Sprite* name_panel    = Scale9Sprite::createWithSpriteFrameName("Title Box.png");
    name_panel->setCapInsets(name_panel_center_rect);
    name_panel->setCascadeOpacityEnabled(true);
    name_panel->setPosition(name_panel_position);
    name_panel->setContentSize(name_panel_size);
    m_craft_panel->addChild(name_panel);

    const Vec2 NAME_PANEL_SIZE = name_panel->getContentSize();

    Vec2 name_label_position = Vec2(NAME_PANEL_SIZE.x * 0.5f, NAME_PANEL_SIZE.y * 0.5f);
    Color32 name_label_color = Color32(186, 145, 88);
    Label* name_label        = Label::createWithTTF("FORGE", "fonts/exqt.ttf", 16);
    name_label->setColor(name_label_color);
    name_label->setPosition(name_label_position);
    name_panel->addChild(name_label);
}

void CraftUI::CreateRecipeLayer()
{
    Vec2 recipe_layer_size     = Vec2(204.f, 220.f);
    Vec2 recipe_layer_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f);
    m_list_layer               = Layer::create();
    m_list_layer->setCascadeOpacityEnabled(true);
    m_list_layer->setPosition(recipe_layer_position);
    m_list_layer->setContentSize(recipe_layer_size);
    m_craft_panel->addChild(m_list_layer);

    const Vec2 LAYER_SIZE = m_list_layer->getContentSize();

    Vec2 scroll_view_size     = Vec2(204.f, 220.f);
    Vec2 scroll_view_position = Vec2(LAYER_SIZE.x * 0.5f, LAYER_SIZE.y * 0.5f - 10);
    m_list_scroll_view        = ScrollView::create();
    m_list_scroll_view->setDirection(ScrollView::Direction::VERTICAL);
    m_list_scroll_view->setBounceEnabled(false);
    m_list_scroll_view->setScrollBarEnabled(false);
    m_list_scroll_view->setContentSize(scroll_view_size);
    m_list_scroll_view->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    m_list_scroll_view->setPosition(scroll_view_position);
    m_list_layer->addChild(m_list_scroll_view);
}

void CraftUI::CreateRecipeSlots()
{
    const std::vector<RecipeData>& recipe_data_vector = m_recipe_data_service->GetRecipeDatas();

    const int SLOT_COUNT       = recipe_data_vector.size();
    const float SLOT_HEIGHT    = 48.0f;
    const float SLOT_SPACE     = 6.0f;
    const float CONTENT_HEIGHT = SLOT_COUNT * SLOT_HEIGHT + (SLOT_COUNT + 1) * SLOT_SPACE;
    const float VIEW_HEIGHT    = m_list_scroll_view->getContentSize().height;

    float content_height = std::max(CONTENT_HEIGHT, VIEW_HEIGHT);

    Size container_size = Size(216.f, content_height);
    m_list_scroll_view->setInnerContainerSize(container_size);

    Layout* inner = m_list_scroll_view->getInnerContainer();

    const float START_Y = content_height - SLOT_SPACE - SLOT_HEIGHT * 0.5f;

    for (int i = 0; i < recipe_data_vector.size(); i++)
    {
        std::optional<ItemData> optional_item_data = m_item_data_service->GetItemData(recipe_data_vector[i].m_product_code);
        if (!optional_item_data.has_value())
            continue;

        RecipeSlot* recipe_slot = RecipeSlot::create();
        recipe_slot->InitUI(optional_item_data.value(), [this](ItemCode item_code) { OnClickedRecipe(item_code); });

        float x = container_size.width * 0.5f - 6;
        float y = START_Y - i * (SLOT_HEIGHT + SLOT_SPACE);

        recipe_slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        recipe_slot->setPosition(Vec2(x, y));

        inner->addChild(recipe_slot);
    }
}

void CraftUI::CreateCraftLayer()
{
    Vec2 craft_layer_size     = m_panel_size;
    Vec2 craft_layer_position = Vec2(0, 0);
    m_craft_layer             = Layer::create();
    m_craft_layer->setCascadeOpacityEnabled(true);
    m_craft_layer->setPosition(craft_layer_position);
    m_craft_layer->setContentSize(craft_layer_size);
    m_craft_panel->addChild(m_craft_layer);

    const Vec2 LAYER_SIZE = m_craft_layer->getContentSize();

    Rect frame_center_rect       = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Vec2 portrait_frame_position = Vec2(46, LAYER_SIZE.y - 76);
    Vec2 portrait_frame_size     = Vec2(48.f, 48.f);
    Scale9Sprite* portrait_frame = Scale9Sprite::createWithSpriteFrameName("Item Box.png");
    portrait_frame->setCascadeOpacityEnabled(true);
    portrait_frame->setContentSize(portrait_frame_size);
    portrait_frame->setPosition(portrait_frame_position);
    portrait_frame->setCapInsets(frame_center_rect);
    m_craft_layer->addChild(portrait_frame);

    const Vec2 FRAME_SIZE = portrait_frame->getContentSize();

    Vec2 portrait_sprite_position = Vec2(FRAME_SIZE.x * 0.5f, FRAME_SIZE.y * 0.5f);
    m_portrait_sprite             = Sprite::create();
    m_portrait_sprite->setPosition(portrait_sprite_position);
    portrait_frame->addChild(m_portrait_sprite);

    Vec2 name_label_position = Vec2(76, LAYER_SIZE.y - 60);
    Vec2 name_label_anchor   = Vec2::ANCHOR_MIDDLE_LEFT;
    m_product_name_label     = Label::createWithTTF("아이템 명", "fonts/exqt.ttf", 12);
    m_product_name_label->setPosition(name_label_position);
    m_product_name_label->setAnchorPoint(name_label_anchor);
    m_product_name_label->setHorizontalAlignment(TextHAlignment::LEFT);
    m_product_name_label->setColor(Color32(89, 148, 104));
    m_craft_layer->addChild(m_product_name_label);

    Vec2 count_label_position = Vec2(76, LAYER_SIZE.y - 72);
    Vec2 count_label_anchor   = Vec2::ANCHOR_MIDDLE_LEFT;
    m_product_count_label     = Label::createWithTTF("x 1", "fonts/exqt.ttf", 12);
    m_product_count_label->setPosition(count_label_position);
    m_product_count_label->setAnchorPoint(count_label_anchor);
    m_product_count_label->setHorizontalAlignment(TextHAlignment::LEFT);
    m_product_count_label->setColor(Color32::WHITE);
    m_craft_layer->addChild(m_product_count_label);

    Vec2 craft_button_position = Vec2(LAYER_SIZE.x - 50, LAYER_SIZE.y - 76);
    m_craft_button             = Button::create();
    m_craft_button->loadTextureNormal("Button.png", Widget::TextureResType::PLIST);
    m_craft_button->setPosition(craft_button_position);
    m_craft_button->setTitleText("제작");
    m_craft_button->setTitleFontSize(12);
    m_craft_button->setTitleFontName("fonts/exqt.ttf");
    m_craft_button->addClickEventListener(AX_CALLBACK_0(CraftUI::OnClickedCraft, this));
    m_craft_layer->addChild(m_craft_button);

    Vec2 back_buton_position = Vec2(LAYER_SIZE.x - 50, LAYER_SIZE.y - 92);
    Button* back_button      = Button::create();
    back_button->loadTextureNormal("Button.png", Widget::TextureResType::PLIST);
    back_button->setPosition(back_buton_position);
    back_button->setTitleText("취소");
    back_button->setTitleFontSize(12);
    back_button->setTitleColor(Color32::BLACK);
    back_button->addClickEventListener([this](Object*)
    {
        ToggleLayer(false);
        SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
    });
    back_button->setTitleFontName("fonts/exqt.ttf");
    m_craft_layer->addChild(back_button);

    Vec2 scroll_view_size     = Vec2(184.f, 150.f);
    Vec2 scroll_view_position = Vec2(LAYER_SIZE.x * 0.5f, LAYER_SIZE.y * 0.5f - 50);
    Rect grid_center_rect     = Rect(4.f, 4.f, 24.f - 4.f * 2, 24.f - 4.f * 2);
    Scale9Sprite* grid_sprite = Scale9Sprite::createWithSpriteFrameName("Item Box.png");
    grid_sprite->setPosition(scroll_view_position);
    grid_sprite->setContentSize(scroll_view_size);
    grid_sprite->setCapInsets(grid_center_rect);
    m_craft_layer->addChild(grid_sprite);

    m_ingredient_scroll_view        = ScrollView::create();
    m_ingredient_scroll_view->setDirection(ScrollView::Direction::VERTICAL);
    m_ingredient_scroll_view->setBounceEnabled(false);
    m_ingredient_scroll_view->setScrollBarEnabled(false);
    m_ingredient_scroll_view->setContentSize(scroll_view_size);
    m_ingredient_scroll_view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ingredient_scroll_view->setPosition(scroll_view_position);
    m_craft_layer->addChild(m_ingredient_scroll_view);
}

void CraftUI::CreateIngredientSlots(ItemCode item_code)
{
    std::optional<RecipeData> optional_recipe = m_recipe_data_service->GetRecipeData(item_code);
    if (!optional_recipe.has_value())
        return;

    RecipeData recipe_data = optional_recipe.value();
    const std::vector<IngredientData>& ingredient_data_vector = recipe_data.m_ingredient_code_vector;

    const int SLOT_COUNT       = ingredient_data_vector.size();
    const float SLOT_HEIGHT    = 48.0f;
    const float SLOT_SPACE     = 6.0f;
    const float CONTENT_HEIGHT = SLOT_COUNT * SLOT_HEIGHT + (SLOT_COUNT + 1) * SLOT_SPACE;
    const float VIEW_HEIGHT    = m_ingredient_scroll_view->getContentSize().height;

    float content_height = std::max(CONTENT_HEIGHT, VIEW_HEIGHT);

    Size container_size = Size(216.f, content_height);
    m_ingredient_scroll_view->setInnerContainerSize(container_size);

    Layout* inner = m_ingredient_scroll_view->getInnerContainer();

    const float START_Y = content_height - SLOT_SPACE - SLOT_HEIGHT * 0.5f;

    m_ingredient_vector.clear();
    for (int i = 0; i < ingredient_data_vector.size(); i++)
    {
        std::optional<ItemData> optional_item_data = m_item_data_service->GetItemData(ingredient_data_vector[i].m_ingredient_item_code);
        if (!optional_item_data.has_value())
            continue;

        IngredientSlot* ingredient_slot = static_cast<IngredientSlot*>(ObjectPoolManager::Get(ObjectType::INGREDIENT_SLOT));
        ingredient_slot->InitUI(optional_item_data.value(), ingredient_data_vector[i], m_inventory_service);

        float x = container_size.width * 0.5f - 16;
        float y = START_Y - i * (SLOT_HEIGHT + SLOT_SPACE);

        ingredient_slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        ingredient_slot->setPosition(Vec2(x, y));

        inner->addChild(ingredient_slot);
        m_ingredient_vector.push_back(ingredient_slot);
    }
}

void CraftUI::ReleaseIngredientSlots()
{
    for (IngredientSlot* ingredient_slot : m_ingredient_vector)
        ObjectPoolManager::Release(ingredient_slot);

    m_ingredient_vector.clear();
}


void CraftUI::ToggleLayer(bool is_craft_mode, bool can_jump)
{
    m_list_layer->setVisible(!is_craft_mode);
    m_craft_layer->setVisible(is_craft_mode);

    if (!is_craft_mode)
    {
        if (can_jump)
            m_list_scroll_view->jumpToTop();

        ReleaseIngredientSlots();
    }
        

    m_ingredient_scroll_view->jumpToTop();
}

void CraftUI::OnClickedRecipe(ItemCode item_code)
{
    m_item_code = item_code;

    ToggleLayer(true);
    CreateIngredientSlots(item_code);
    InitCrafter(item_code);

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
}

void CraftUI::OnClickedCraft()
{
    if (!CanCraft())
        return;

    std::optional<RecipeData> optional_recipe = m_recipe_data_service->GetRecipeData(m_item_code);
    if (!optional_recipe.has_value())
        return;

    RecipeData recipe_data                                    = optional_recipe.value();
    const std::vector<IngredientData>& ingredient_data_vector = recipe_data.m_ingredient_code_vector;

    for (IngredientData ingredient_data : ingredient_data_vector)
        m_inventory_service->DecreaseItem(ingredient_data.m_ingredient_item_code, ingredient_data.m_ingredient_count);

    m_inventory_service->IncreaseItem(recipe_data.m_product_code, recipe_data.m_product_count);

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Craft.mp3");
}

void CraftUI::InitCrafter(ItemCode item_code)
{
    std::optional<ItemData> optional_item_data = m_item_data_service->GetItemData(item_code);
    if (!optional_item_data.has_value())
        return;

    ItemData item_data = optional_item_data.value();

    m_portrait_sprite->setSpriteFrame(item_data.m_frame_name);
    m_product_name_label->setString(item_data.m_item_name);

    std::optional<RecipeData> optional_recipe_data = m_recipe_data_service->GetRecipeData(item_code);
    if (!optional_recipe_data.has_value())
        return;

    RecipeData recipe_data = optional_recipe_data.value();

    m_product_count_label->setString(fmt::format("x {}", recipe_data.m_product_count));

    Label* title_label = m_craft_button->getTitleLabel();
    bool can_craft     = CanCraft();

    title_label->setColor(can_craft ? Color32::BLACK : Color32::RED);
    m_craft_button->setEnabled(can_craft);
}

void CraftUI::UpdateUI(ax::EventCustom* event)
{
    ItemSlotData item_slot_data = *static_cast<ItemSlotData*>(event->getUserData());
    if (m_item_code != item_slot_data.m_item_code)
        return;

    Label* title_label = m_craft_button->getTitleLabel();
    bool can_craft     = CanCraft(); 

    title_label->setColor(can_craft? Color32::BLACK : Color32::RED);
    m_craft_button->setEnabled(can_craft);
}

bool CraftUI::CanCraft() const
{
    std::optional<RecipeData> optional_recipe = m_recipe_data_service->GetRecipeData(m_item_code);
    if (!optional_recipe.has_value())
        return false;

    RecipeData recipe_data                                    = optional_recipe.value();
    const std::vector<IngredientData>& ingredient_data_vector = recipe_data.m_ingredient_code_vector;

    for (IngredientData ingredient_data : ingredient_data_vector)
    {
        if (ingredient_data.m_ingredient_count > m_inventory_service->GetItemCount(ingredient_data.m_ingredient_item_code))
            return false;
    }

    return true;
}
