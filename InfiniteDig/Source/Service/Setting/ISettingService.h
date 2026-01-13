#pragma once

#include <optional>
#include "Service/ISaveable.h"
#include "Data/Setting/SettingData.h"

namespace Service
{
    class ISettingService : public ISaveable
    {
    public:
        virtual ~ISettingService() = default;

        virtual void SetAudio(AudioType audio_type, const AudioSetting& audio_setting) = 0;
        virtual std::optional<AudioSetting> GetAudio(AudioType audio_type) const       = 0;
        virtual float GetEffectiveVolume(AudioType audio_type) const                   = 0;
    };
}
