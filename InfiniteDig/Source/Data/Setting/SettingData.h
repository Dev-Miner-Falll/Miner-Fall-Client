#pragma once

enum class AudioType
{
    BGM = 0,
    SFX,
};

struct AudioSetting
{
    bool m_enabled;
    float m_volume;
};

struct SettingData
{
    AudioSetting m_bgm_setting;
    AudioSetting m_sfx_setting;

    SettingData()
        : m_bgm_setting(true, 0.75f)
        , m_sfx_setting(true, 0.75f)
    {}

    SettingData(const AudioSetting& bgm_setting, const AudioSetting& sfx_setting)
        : m_bgm_setting(bgm_setting)
        , m_sfx_setting(sfx_setting)
    {}

    SettingData(const SettingData& setting_data)
        : m_bgm_setting(setting_data.m_bgm_setting)
        , m_sfx_setting(setting_data.m_sfx_setting)
    {}
};
