#pragma once

#include "ISettingService.h"

namespace Service
{
    class LocalSettingService : public ISettingService
    {
    public:
        void SetAudio(AudioType audio_type, const AudioSetting& audio_setting) override;
        std::optional<AudioSetting> GetAudio(AudioType audio_type) const override;
        float GetEffectiveVolume(AudioType audio_type) const override;

    public:
        void Load() override;
        void Save() override;

    public:
        LocalSettingService();

        LocalSettingService(const LocalSettingService&)              = delete;
        LocalSettingService& operator=(const LocalSettingService&) = delete;

    private:
        SettingData m_setting_data;
    };
}
