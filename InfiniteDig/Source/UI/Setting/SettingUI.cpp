#include "SettingUI.h"
#include "axmol/ui/UIButton.h"
#include "Utils/Manager/SoundManager.h"

using namespace ax::ui;
USING_NS_AX;

SettingUI* SettingUI::create(std::shared_ptr<Service::ISettingService> setting_service)
{
    SettingUI* setting_ui = new (std::nothrow) SettingUI();
    if (setting_ui && setting_ui->init(setting_service))
    {
        setting_ui->autorelease();
        return setting_ui;
    }

    AX_SAFE_DELETE(setting_ui);
    return nullptr;
}

bool SettingUI::init(std::shared_ptr<Service::ISettingService> setting_service)
{
    if (!Layer::init())
        return false;

    m_setting_service = setting_service;

    CreateUI();

    return true;
}

void SettingUI::onEnter()
{
    Layer::onEnter();
    InitUI();
}



void SettingUI::OpenUI()
{
    ToggleUI(true);
}

void SettingUI::CloseUI()
{
    ToggleUI(false);
}

void SettingUI::ToggleUI(bool active)
{
    const float TARGET_TIME = 0.25f;

    this->stopAllActions();

    if (active)
    {
        CallFunc* enable_action   = CallFunc::create([this]() { this->setVisible(true); });
        FadeIn* fade_in_action    = FadeIn::create(TARGET_TIME);
        Sequence* sequence_action = Sequence::create(enable_action, fade_in_action, nullptr);

        m_block_listener = EventListenerTouchOneByOne::create();
        m_block_listener->setSwallowTouches(true);
        m_block_listener->onTouchBegan = [](Touch*, Event*) { return true; };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_block_listener, m_block_layer);

        this->runAction(sequence_action);
    }
    else
    {
        FadeOut* fade_out_action  = FadeOut::create(TARGET_TIME);
        CallFunc* disable_action  = CallFunc::create([this]() { this->setVisible(false); });
        Sequence* sequence_action = Sequence::create(fade_out_action, disable_action, nullptr);

        if (m_block_listener)
        {
            _eventDispatcher->removeEventListener(m_block_listener);
            m_block_listener = nullptr;
        }

        this->runAction(sequence_action);
    }
}



void SettingUI::InitUI()
{
    //if (!m_setting_service)
    //    return;

    std::optional<AudioSetting> optional_bgm = m_setting_service->GetAudio(AudioType::BGM);
    if (optional_bgm.has_value())
    {
        AudioSetting bgm_setting = optional_bgm.value();

        m_bgm_print_toggle->setSelectedIndex(bgm_setting.m_enabled ? 0 : 1);
        m_bgm_volume_slider->setPercent(bgm_setting.m_volume * 100.0f);
    }

    std::optional<AudioSetting> optional_sfx = m_setting_service->GetAudio(AudioType::SFX);
    if (optional_sfx.has_value())
    {
        AudioSetting sfx_setting = optional_sfx.value();
        m_sfx_print_toggle->setSelectedIndex(sfx_setting.m_enabled ? 0 : 1);
        m_sfx_volume_slider->setPercent(sfx_setting.m_volume * 100.0f);
    }
}



void SettingUI::CreateUI()
{
    m_canvas_size = _director->getCanvasSize();

    this->setCascadeOpacityEnabled(true);
    this->setVisible(false);
    this->setOpacity(0.0f);

    CreateBlockingLayer();
    CreateSettingPanel();
    CreateToggleGroup();
    CreateTitleGroup();
    CreateToggleGroup();
    CreateSliderGroup();
    CreateExitButton();
}

void SettingUI::CreateBlockingLayer()
{
    m_block_layer = Layer::create();
    m_block_layer->setOpacity(0.5f);
    m_block_layer->setContentSize(m_canvas_size);
    m_block_layer->setPosition(Vec2::ZERO);
    m_block_layer->setCascadeOpacityEnabled(true);
    this->addChild(m_block_layer);
}

void SettingUI::CreateSettingPanel()
{
    Rect setting_panel_center_rect = Rect(15.f, 15.f, 96.f - 15.f * 2, 96.f - 15.f * 2);
    Vec2 setting_panel_position    = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5f);
    Size setting_panel_size        = Size(192, 192);
    m_setting_panel                = Scale9Sprite::createWithSpriteFrameName("Base Panel.png");
    m_setting_panel->setCapInsets(setting_panel_center_rect);
    m_setting_panel->setCascadeOpacityEnabled(true);
    m_setting_panel->setContentSize(setting_panel_size);
    m_setting_panel->setPosition(setting_panel_position);
    this->addChild(m_setting_panel);

    m_panel_size = m_setting_panel->getContentSize();

    Vec2 pattern_position  = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 28.f);
    Vec2 pattern_size      = Vec2(166, 16);
    Sprite* pattern_sprite = Sprite::createWithSpriteFrameName("Top Pattern.png");
    pattern_sprite->setPosition(pattern_position);
    pattern_sprite->setContentSize(pattern_size);
    m_setting_panel->addChild(pattern_sprite);

    float dx[] = {0, m_panel_size.x};
    float dy[] = {0, m_panel_size.y};

    for (int i = 0; i < 2; i++)
    {
        float x_offset = 5.f * (i % 2 == 0 ? 1 : -1);

        for (int j = 0; j < 2; j++)
        {
            float y_offset        = 5.f * (j % 2 == 0 ? 1 : -1);
            Vec2 corner_position  = Vec2(dx[i] + x_offset, dy[j] + y_offset);
            Sprite* corner_sprite = Sprite::createWithSpriteFrameName("Knot.png");
            corner_sprite->setPosition(corner_position);
            m_setting_panel->addChild(corner_sprite);
        }
    }

    Vec2 close_button_position = Vec2(m_panel_size.x - 5.f, m_panel_size.y - 5.f);
    Vec2 close_button_size     = Vec2(21.f, 21.f);
    Button* close_button       = Button::create();
    close_button->loadTextureNormal("Close Button.png", Widget::TextureResType::PLIST);
    close_button->setPosition(close_button_position);
    close_button->setSizePercent(close_button_size);
    close_button->addClickEventListener([this](Object*)
    {
        SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
        this->CloseUI();
    });
    m_setting_panel->addChild(close_button);
}

void SettingUI::CreateTitleGroup()
{
    Rect name_panel_center_rect = Rect(4.f, 2.f, 64.f - 4.f * 2, 16.f - 2.f * 2);
    Vec2 name_panel_position    = Vec2(m_panel_size.x * 0.5f, m_panel_size.y - 4);
    Size name_panel_size        = Size(96, 24);
    Scale9Sprite* name_panel    = Scale9Sprite::createWithSpriteFrameName("Title Box.png");
    name_panel->setCapInsets(name_panel_center_rect);
    name_panel->setCascadeOpacityEnabled(true);
    name_panel->setContentSize(name_panel_size);
    name_panel->setPosition(name_panel_position);
    m_setting_panel->addChild(name_panel);

    const Vec2 NAME_PANEL_SIZE = name_panel->getContentSize();

    Vec2 title_label_position = Vec2(NAME_PANEL_SIZE.x * 0.5f, NAME_PANEL_SIZE.y * 0.5f);
    Color32 title_label_color = Color32(186, 145, 88);
    Label* title_label        = Label::createWithTTF("SETTING", "fonts/exqt.ttf", 16);
    title_label->setCascadeOpacityEnabled(true);
    title_label->setColor(title_label_color);
    title_label->setPosition(title_label_position);
    name_panel->addChild(title_label);
}

void SettingUI::CreateToggleGroup()
{
    Vec2 bgm_print_label_position = Vec2(m_panel_size.x * 0.5f - 44.f, m_panel_size.y * 0.5f + 40.f);
    Color32 bgm_print_label_color = Color32(186, 145, 88);
    Label* bgm_print_label        = Label::createWithTTF("배경음 출력", "fonts/exqt.ttf", 12);
    bgm_print_label->getFontAtlas()->setAliasTexParameters();
    bgm_print_label->setColor(bgm_print_label_color);
    bgm_print_label->setPosition(bgm_print_label_position);
    m_setting_panel->addChild(bgm_print_label);

    Vec2 sfx_print_label_position = Vec2(m_panel_size.x * 0.5f + 44.f, m_panel_size.y * 0.5f + 40.f);
    Color32 sfx_print_label_color = Color32(186, 145, 88);
    Label* sfx_print_label        = Label::createWithTTF("효과음 출력", "fonts/exqt.ttf", 12);
    sfx_print_label->getFontAtlas()->setAliasTexParameters();
    sfx_print_label->setColor(sfx_print_label_color);
    sfx_print_label->setPosition(sfx_print_label_position);
    m_setting_panel->addChild(sfx_print_label);

    Sprite* bgm_print_on_sprite       = Sprite::createWithSpriteFrameName("On Toggle.png");
    MenuItemSprite* bgm_print_on_item = MenuItemSprite::create(bgm_print_on_sprite, bgm_print_on_sprite);
    bgm_print_on_item->setCascadeOpacityEnabled(true);

    Sprite* bgm_print_off_sprite      = Sprite::createWithSpriteFrameName("Off Toggle.png");
    MenuItemSprite* bgm_print_off_item = MenuItemSprite::create(bgm_print_off_sprite, bgm_print_off_sprite);
    bgm_print_off_item->setCascadeOpacityEnabled(true);

    m_bgm_print_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_1(SettingUI::OnClickedBGMPrintToggle, this),
                                                            bgm_print_on_item, bgm_print_off_item, nullptr);
    m_bgm_print_toggle->setCascadeOpacityEnabled(true);

    Sprite* sfx_print_on_sprite       = Sprite::createWithSpriteFrameName("On Toggle.png");
    MenuItemSprite* sfx_print_on_item = MenuItemSprite::create(sfx_print_on_sprite, sfx_print_on_sprite);
    sfx_print_on_item->setCascadeOpacityEnabled(true);

    Sprite* sfx_print_off_sprite       = Sprite::createWithSpriteFrameName("Off Toggle.png");
    MenuItemSprite* sfx_print_off_item = MenuItemSprite::create(sfx_print_off_sprite, sfx_print_off_sprite);
    sfx_print_off_item->setCascadeOpacityEnabled(true);

    m_sfx_print_toggle = MenuItemToggle::createWithCallback(AX_CALLBACK_1(SettingUI::OnClickedSFXPrintToggle, this),
                                                            sfx_print_on_item, sfx_print_off_item, nullptr);
    m_sfx_print_toggle->setCascadeOpacityEnabled(true);

    Vec2 toggle_menu_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5 + 20);
    Menu* toggle_menu         = Menu::create(m_bgm_print_toggle, m_sfx_print_toggle, nullptr);
    toggle_menu->setCascadeOpacityEnabled(true);
    toggle_menu->setPosition(toggle_menu_position);
    toggle_menu->alignItemsHorizontallyWithPadding(36);
    m_setting_panel->addChild(toggle_menu);
}

void SettingUI::CreateSliderGroup()
{
    Vec2 bgm_volume_label_position = Vec2(m_panel_size.x * 0.5f - 54.f, m_panel_size.y * 0.5f - 10.f);
    Color32 bgm_volume_label_color = Color32(186, 145, 88);
    Label* bgm_volume_label        = Label::createWithTTF("배경음", "fonts/exqt.ttf", 12);
    bgm_volume_label->getFontAtlas()->setAliasTexParameters();
    bgm_volume_label->setColor(bgm_volume_label_color);
    bgm_volume_label->setPosition(bgm_volume_label_position);
    m_setting_panel->addChild(bgm_volume_label);

    Vec2 sfx_volume_label_position = Vec2(m_panel_size.x * 0.5f - 54.f, m_panel_size.y * 0.5f - 30.f);
    Color32 sfx_volume_label_color = Color32(186, 145, 88);
    Label* sfx_volume_label        = Label::createWithTTF("효과음", "fonts/exqt.ttf", 12);
    sfx_volume_label->getFontAtlas()->setAliasTexParameters();
    sfx_volume_label->setColor(sfx_volume_label_color);
    sfx_volume_label->setPosition(sfx_volume_label_position);
    m_setting_panel->addChild(sfx_volume_label);

    Vec2 bgm_slider_position = Vec2(m_panel_size.x * 0.5f + 20.f, m_panel_size.y * 0.5f - 10.f);
    Vec2 bgm_slider_size     = Vec2(96.f, 12.f);
    m_bgm_volume_slider      = Slider::create();
    m_bgm_volume_slider->setScale9Enabled(true);
    m_bgm_volume_slider->setContentSize(bgm_slider_size);
    m_bgm_volume_slider->loadBarTexture("Slider Frame.png", Widget::TextureResType::PLIST);
    m_bgm_volume_slider->loadSlidBallTextureNormal("Knot.png", Widget::TextureResType::PLIST);
    m_bgm_volume_slider->loadProgressBarTexture("Slider Inner.png", Widget::TextureResType::PLIST);
    m_bgm_volume_slider->setPosition(bgm_slider_position);
    m_bgm_volume_slider->addEventListener(AX_CALLBACK_2(SettingUI::OnBGMVolumeChanged, this));
    m_setting_panel->addChild(m_bgm_volume_slider);

    Vec2 sfx_slider_position = Vec2(m_panel_size.x * 0.5f + 20, m_panel_size.y * 0.5f - 30);
    Vec2 sfx_slider_size     = Vec2(96, 12);
    m_sfx_volume_slider      = Slider::create();
    m_sfx_volume_slider->setScale9Enabled(true);
    m_sfx_volume_slider->setContentSize(sfx_slider_size);
    m_sfx_volume_slider->loadBarTexture("Slider Frame.png", Widget::TextureResType::PLIST);
    m_sfx_volume_slider->loadSlidBallTextureNormal("Knot.png", Widget::TextureResType::PLIST);
    m_sfx_volume_slider->loadProgressBarTexture("Slider Inner.png", Widget::TextureResType::PLIST);
    m_sfx_volume_slider->setPosition(sfx_slider_position);
    m_sfx_volume_slider->addEventListener(AX_CALLBACK_2(SettingUI::OnSFXVolumeChanged, this));
    m_setting_panel->addChild(m_sfx_volume_slider);
}

void SettingUI::CreateExitButton()
{
    Vec2 exit_button_position = Vec2(m_panel_size.x * 0.5f, m_panel_size.y * 0.5f - 70.f);
    Button* exit_button       = Button::create();
    exit_button->loadTextureNormal("Button.png", Widget::TextureResType::PLIST);
    exit_button->setTitleFontName("fonts/exqt.ttf");
    exit_button->setTitleFontSize(12);
    exit_button->setTitleColor(Color32(87, 72, 82));
    exit_button->setTitleText("종료");
    exit_button->setPosition(exit_button_position);
    exit_button->addClickEventListener(AX_CALLBACK_0(SettingUI::OnClickedExit, this));
    m_setting_panel->addChild(exit_button);
}



void SettingUI::OnClickedBGMPrintToggle(Object* sender)
{
    MenuItemToggle* toggle = static_cast<MenuItemToggle*>(sender);
    bool is_on             = (toggle->getSelectedIndex() == 0);

    std::optional<AudioSetting> optional_bgm = m_setting_service->GetAudio(AudioType::BGM);
    if (!optional_bgm.has_value())
        return;

    AudioSetting bgm_setting = optional_bgm.value();
    m_setting_service->SetAudio(AudioType::BGM, AudioSetting(is_on, bgm_setting.m_volume));

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
}

void SettingUI::OnClickedSFXPrintToggle(Object* sender)
{
    MenuItemToggle* toggle = static_cast<MenuItemToggle*>(sender);
    bool is_on             = (toggle->getSelectedIndex() == 0);

    std::optional<AudioSetting> optional_sfx = m_setting_service->GetAudio(AudioType::SFX);
    if (!optional_sfx.has_value())
        return;

    AudioSetting sfx_setting = optional_sfx.value();
    m_setting_service->SetAudio(AudioType::SFX, AudioSetting(is_on, sfx_setting.m_volume));

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Button Interaction.mp3");
}

void SettingUI::OnBGMVolumeChanged(Object* sender, ax::ui::Slider::EventType type)
{
    if (type != Slider::EventType::ON_PERCENTAGE_CHANGED)
        return;

    ui::Slider* slider = static_cast<Slider*>(sender);
    float volume       = slider->getPercent() / 100.0f;

    std::optional<AudioSetting> optional_bgm = m_setting_service->GetAudio(AudioType::BGM);
    if (!optional_bgm.has_value())
        return;

    AudioSetting bgm_setting = optional_bgm.value();

    m_setting_service->SetAudio(AudioType::BGM, AudioSetting(bgm_setting.m_enabled, volume));
}

void SettingUI::OnSFXVolumeChanged(Object* sender, ax::ui::Slider::EventType type)
{
    if (type != Slider::EventType::ON_PERCENTAGE_CHANGED)
        return;

    ui::Slider* slider = static_cast<Slider*>(sender);
    float volume       = slider->getPercent() / 100.0f;

    std::optional<AudioSetting> optional_sfx = m_setting_service->GetAudio(AudioType::SFX);
    if (!optional_sfx.has_value())
        return;

    AudioSetting sfx_setting = optional_sfx.value();

    m_setting_service->SetAudio(AudioType::SFX, AudioSetting(sfx_setting.m_enabled, volume));
}

void SettingUI::OnClickedExit()
{
    _director->end();
}
