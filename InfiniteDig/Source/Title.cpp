#include "Title.h"
#include "Game.h"
#include "Service/ServiceLocator.h"
#include "Utils/Pool/ObjectPoolManager.h"
#include "Utils/Manager/SoundManager.h"
#include "Game/BackgroundManager.h"

USING_NS_AX;

bool Title::init()
{
    if (!Scene::init())
        return false;

    RegisterSpriteFrame();
    ObjectPoolManager::Initialize();
    CreateLayer();
    CreateUI();

    return true;
}

void Title::onEnter()
{
    Scene::onEnter();

    SoundManager::getInstance()->PlayBGM("Sounds/BGM/Title BGM.mp3");

    m_touch_listener = EventListenerTouchOneByOne::create();
    m_touch_listener->onTouchBegan = [=](Touch* , Event*)
    {
        SoundManager::getInstance()->PlaySFX("Sounds/SFX/Toggle Interaction.mp3");

        Game* game_scene                   = Game::create();
        TransitionFadeTR* scene_transition = TransitionFadeTR::create(1.0f, game_scene);

        _director->replaceScene(scene_transition);
        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touch_listener, this);
}

void Title::onExit()
{
    if (m_touch_listener)
    {
        _eventDispatcher->removeEventListener(m_touch_listener);
        m_touch_listener = nullptr;
    }

    Scene::onExit();
}



void Title::RegisterSpriteFrame()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Game/Object/Tileset.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Game/Background/Background.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Game/UI/UI.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Game/UI/Items.plist");
}

void Title::CreateLayer()
{
    m_background_layer = Layer::create();
    this->addChild(m_background_layer);

    m_ui_layer = Layer::create();
    this->addChild(m_ui_layer);
}


void Title::CreateUI()
{
    m_canvas_size = _director->getCanvasSize();

    CreateBackground();
    CreateTitleLogo();
    CreateTouchLabel();
}

void Title::CreateBackground()
{
    BackgroundManager* background_manager = BackgroundManager::create(ServiceLocator::Get<Service::IBackgroundDataService>());
    m_background_layer->addChild(background_manager);
    background_manager->ReplaceBackground("Cave BG.png", 32, 0.f);
}

void Title::CreateTitleLogo()
{
    Vec2 logo_position = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5 + 100.f);
    Sprite* logo_sprite = Sprite::create("Title/Title Logo.png");
    logo_sprite->setPosition(logo_position);
    logo_sprite->setScale(0.75f);
    m_ui_layer->addChild(logo_sprite);

    ScaleTo* scale_max_action = ScaleTo::create(1.5f, 0.8f);
    ScaleTo* scale_min_action = ScaleTo::create(1.5f, 0.75f);
    Sequence* sequence_action = Sequence::create(scale_max_action, scale_min_action, nullptr);
    RepeatForever* repeat_action = RepeatForever::create(sequence_action);
    logo_sprite->runAction(repeat_action);
}

void Title::CreateTouchLabel()
{
    Vec2 label_position = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5 - 150.f);
    Label* start_label  = Label::createWithTTF("게임을 시작하려면 터치하세요.", "fonts/exqt.ttf", 16);
    start_label->getFontAtlas()->setAliasTexParameters();
    start_label->setPosition(label_position);
    start_label->setColor(Color32(208, 176, 119));
    m_ui_layer->addChild(start_label);


    Vec2 up_position       = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5 - 145.f);
    Vec2 down_position     = Vec2(m_canvas_size.x * 0.5f, m_canvas_size.y * 0.5 - 150.f);

    DelayTime* delay_action   = DelayTime::create(1.0f);
    MoveTo* move_start_action = MoveTo::create(0.2f, up_position);
    MoveTo* move_end_action   = MoveTo::create(0.2f, down_position);
    Sequence* sequence_action = Sequence::create(delay_action,
                                                 move_start_action,
                                                 move_end_action,
                                                 move_start_action,
                                                 move_end_action,
                                                 nullptr);
    RepeatForever* repeat_action = RepeatForever::create(sequence_action);
    start_label->runAction(repeat_action);
}
