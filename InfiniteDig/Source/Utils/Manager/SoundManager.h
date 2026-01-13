#pragma once

#include <unordered_map>
#include "axmol/axmol.h"
#include "axmol/audio/AudioEngine.h"
#include "Service/Setting/ISettingService.h"
#include "Utils/Singleton.h"

struct SoundChannel
{
    int m_max_play_count = 4;
    std::vector<int> m_sfx_id_vector;
};

class SoundManager : public Singleton<SoundManager>
{
public:
    CREATE_FUNC(SoundManager)
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void Inject(std::shared_ptr<Service::ISettingService> setting_service);

public:
    void PlayBGM(const std::string& bgm_path, float fade_time = 0.5f);
    void PlaySFX(const std::string& sfx_path);

private:
    void RegisterSFXToMap(const std::string& sfx_name, int max_count);
    void InitSFXToMap();

    void UpdateVolume(ax::EventCustom* event);

    void SwapAndFadeInBGM(float duration);
    void FadeInCurrentBGM(float duration);
    void FadeOutCurrentBGM(float duration);
    void StopAllBGM(bool immediately);
    void Schedule(const std::function<void(float)>& callback, float interval, const std::string& key);
    void Unschedule(const std::string& key);

private:
    std::shared_ptr<Service::ISettingService> m_setting_service;
    ax::EventListenerCustom* m_update_listener;
    int m_current_bgm_id;
    int m_next_bgm_id;

    std::unordered_map<std::string, SoundChannel> m_channel_map;
};
