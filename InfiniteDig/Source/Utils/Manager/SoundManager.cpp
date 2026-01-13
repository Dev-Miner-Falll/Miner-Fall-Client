#include "SoundManager.h"

USING_NS_AX;

bool SoundManager::init()
{
    if (!Node::init())
        return false;

    m_current_bgm_id = AudioEngine::INVALID_AUDIO_ID;
    m_next_bgm_id    = AudioEngine::INVALID_AUDIO_ID;

    InitSFXToMap();

    return true;
}

void SoundManager::onEnter()
{
    Node::onEnter();

    m_update_listener = EventListenerCustom::create("BGM_UPDATE", AX_CALLBACK_1(SoundManager::UpdateVolume, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_update_listener, this);
}

void SoundManager::onExit()
{
    if (m_update_listener)
    {
        _eventDispatcher->removeEventListener(m_update_listener);
        m_update_listener = nullptr;
    }

    Node::onExit();
}

void SoundManager::Inject(std::shared_ptr<Service::ISettingService> setting_service)
{
    m_setting_service = setting_service;
}



void SoundManager::PlayBGM(const std::string& bgm_path, float fade_time)
{
    if (!m_setting_service)
        return;

    if (m_next_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        return;

    m_next_bgm_id = AudioEngine::play2d(bgm_path, true, 0.f);

    if (m_current_bgm_id != AudioEngine::INVALID_AUDIO_ID)
    {
        FadeOutCurrentBGM(fade_time);
    }
    else
    {
        SwapAndFadeInBGM(fade_time);
    }
}

void SoundManager::PlaySFX(const std::string& sfx_path)
{
    if (!m_setting_service)
        return;

    float sfx_volume = m_setting_service->GetEffectiveVolume(AudioType::SFX);
    if (sfx_volume <= 0.f)
        return;

    SoundChannel& channel = m_channel_map.at(sfx_path);

    if (channel.m_sfx_id_vector.size() >= channel.m_max_play_count)
        return;

    int id = AudioEngine::play2d(sfx_path, false, sfx_volume);
    channel.m_sfx_id_vector.push_back(id);

    AudioEngine::setFinishCallback(id,
                                   [&](int id, std::string_view sfx_path)
                                   {
                                        auto& vec = m_channel_map[sfx_path.data()].m_sfx_id_vector;
                                        vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
                                   });
}



void SoundManager::SwapAndFadeInBGM(float duration)
{
    m_current_bgm_id = m_next_bgm_id;
    m_next_bgm_id    = AudioEngine::INVALID_AUDIO_ID;

    FadeInCurrentBGM(duration);
}

void SoundManager::FadeInCurrentBGM(float duration)
{
    if (m_current_bgm_id == AudioEngine::INVALID_AUDIO_ID || !m_setting_service)
        return;

    float target_volume = m_setting_service->GetEffectiveVolume(AudioType::BGM) * 100.f;
    float step          = 0.05f;
    float increment     = target_volume * step / duration;

    this->schedule([this, increment, target_volume](float delta)
    {
        float volume = AudioEngine::getVolume(m_current_bgm_id);
        volume += increment * delta;

        if (volume >= target_volume)
        {
            AudioEngine::setVolume(m_current_bgm_id, target_volume);
            this->unschedule("bgm_fade_in");
        }
        else
        {
            AudioEngine::setVolume(m_current_bgm_id, volume);
        }
    }, step, "bgm_fade_in");
}

void SoundManager::FadeOutCurrentBGM(float duration)
{
    int fading_id = m_current_bgm_id;

    float step         = 0.05f;
    float start_volume = AudioEngine::getVolume(fading_id);
    float decrement    = start_volume / duration;

    this->schedule([this, fading_id, decrement, duration](float delta)
    {
        float volume = AudioEngine::getVolume(fading_id);
        volume -= decrement * delta;

        if (volume <= 0.0f)
        {
            AudioEngine::stop(fading_id);
            this->unschedule("bgm_fade_out");

            m_current_bgm_id = AudioEngine::INVALID_AUDIO_ID;

            SwapAndFadeInBGM(duration);
        }
        else
        {
            AudioEngine::setVolume(fading_id, volume);
        }
    }, step, "bgm_fade_out");
}

void SoundManager::StopAllBGM(bool immediately)
{
    this->unschedule("bgm_fade_in");
    this->unschedule("bgm_fade_out");

    if (m_current_bgm_id != AudioEngine::INVALID_AUDIO_ID)
    {
        AudioEngine::stop(m_current_bgm_id);
        m_current_bgm_id = AudioEngine::INVALID_AUDIO_ID;
    }

    if (m_next_bgm_id != AudioEngine::INVALID_AUDIO_ID)
    {
        AudioEngine::stop(m_next_bgm_id);
        m_next_bgm_id = AudioEngine::INVALID_AUDIO_ID;
    }
}



void SoundManager::Schedule(const std::function<void(float)>& callback, float interval, const std::string& key)
{
    Director::getInstance()->getScheduler()->schedule([callback](float dt){callback(dt);}, this, interval, false, key);
}

void SoundManager::Unschedule(const std::string& key)
{
    Director::getInstance()->getScheduler()->unschedule(key, this);
}



void SoundManager::RegisterSFXToMap(const std::string& sfx_name, int max_count)
{
    std::string default_path                                = "Sounds/SFX/";
    m_channel_map[default_path + sfx_name].m_max_play_count = max_count;
}

void SoundManager::InitSFXToMap()
{
    RegisterSFXToMap("Ignition.mp3", 4);
    RegisterSFXToMap("Bomb.mp3", 2);
    RegisterSFXToMap("Break.mp3", 5);
    RegisterSFXToMap("Purchase.mp3", 2);
    RegisterSFXToMap("Small Dynamite.mp3", 3);
    RegisterSFXToMap("Button Interaction.mp3", 3);
    RegisterSFXToMap("Toggle Interaction2.mp3", 3);
    RegisterSFXToMap("Toggle Interaction.mp3", 1);
    RegisterSFXToMap("Disabled Button.mp3", 3);
    RegisterSFXToMap("Craft.mp3", 5);
}


void SoundManager::UpdateVolume(EventCustom* event)
{
    if (!m_setting_service)
        return;

    AudioSetting bgm_setting = *static_cast<AudioSetting*>(event->getUserData());

    if (m_current_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        AudioEngine::setVolume(m_current_bgm_id, bgm_setting.m_enabled ? bgm_setting.m_volume : 0.f);

    if (m_next_bgm_id != AudioEngine::INVALID_AUDIO_ID)
        AudioEngine::setVolume(m_next_bgm_id, bgm_setting.m_enabled ? bgm_setting.m_volume : 0.f);
}
