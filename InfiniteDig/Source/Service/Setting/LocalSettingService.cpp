#include "LocalSettingService.h"
#include "axmol/axmol.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace Service
{

    LocalSettingService::LocalSettingService()
    {
        Load();
    }

    void LocalSettingService::SetAudio(AudioType audio_type, const AudioSetting& audio_setting)
    {
        USING_NS_AX;

        switch (audio_type)
        {
        case AudioType::BGM:
            m_setting_data.m_bgm_setting = audio_setting;
            break;

        case AudioType::SFX:
            m_setting_data.m_sfx_setting = audio_setting;
            break;

        default:
            break;
        }

        if (AudioType::BGM == audio_type)
        {
            EventCustom event("BGM_UPDATE");
            event.setUserData(&m_setting_data.m_bgm_setting);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }

    std::optional<AudioSetting> LocalSettingService::GetAudio(AudioType audio_type) const
    {
        using namespace std;

        switch (audio_type)
        {
        case AudioType::BGM:
            return m_setting_data.m_bgm_setting;

        case AudioType::SFX:
            return m_setting_data.m_sfx_setting;

        default:
            return nullopt;
        }
    }

    float LocalSettingService::GetEffectiveVolume(AudioType audio_type) const
    {
        switch (audio_type)
        {
        case AudioType::BGM:
            return m_setting_data.m_bgm_setting.m_enabled ? m_setting_data.m_bgm_setting.m_volume : 0.f;

        case AudioType::SFX:
            return m_setting_data.m_sfx_setting.m_enabled ? m_setting_data.m_sfx_setting.m_volume : 0.f;

        default:
            return 0.f;
        }
    }



    void LocalSettingService::Load()
    {
        using namespace std;
        using namespace rapidjson;

        const string file_path = ax::FileUtils::getInstance()->getWritablePath() + "SettingData.json";
        if (!ax::FileUtils::getInstance()->isFileExist(file_path))
        {
            Save();
            return;
        }

        string json = ax::FileUtils::getInstance()->getStringFromFile(file_path);

        Document doc;
        doc.Parse(json.c_str());

        if (!doc.IsObject() || !doc.HasMember("audio"))
            return;

        const Value& audio = doc["audio"];

        auto read_audio = [&](const char* name, AudioSetting& audio_setting)
        {
            if (!audio.HasMember(name))
                return;

            const Value& obj = audio[name];
            if (!obj.IsObject())
                return;

            if (obj.HasMember("enabled") && obj["enabled"].IsBool())
                audio_setting.m_enabled = obj["enabled"].GetBool();

            if (obj.HasMember("volume") && obj["volume"].IsNumber())
                audio_setting.m_volume = obj["volume"].GetFloat();
        };

        read_audio("bgm", m_setting_data.m_bgm_setting);
        read_audio("sfx", m_setting_data.m_sfx_setting);
    }

    void LocalSettingService::Save()
    {
        using namespace std;
        using namespace rapidjson;

        Document doc;
        doc.SetObject();

        auto& allocator = doc.GetAllocator();

        Value audio(kObjectType);
        auto write_audio = [&](const char* name, const AudioSetting& audio_setting)
        {
            Value audio_object(kObjectType);

            audio_object.AddMember("enabled", audio_setting.m_enabled, allocator);
            audio_object.AddMember("volume", audio_setting.m_volume, allocator);
            audio.AddMember(Value(name, allocator), audio_object, allocator);
        };

        write_audio("bgm", m_setting_data.m_bgm_setting);
        write_audio("sfx", m_setting_data.m_sfx_setting);

        doc.AddMember("audio", audio, allocator);

        StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);
        doc.Accept(writer);

        const string path = ax::FileUtils::getInstance()->getWritablePath() + "SettingData.json";
        ax::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), path);
    }
}
