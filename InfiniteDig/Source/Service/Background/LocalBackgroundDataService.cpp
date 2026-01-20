#include "axmol/axmol.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "LocalBackgroundDataService.h"

namespace Service
{
    LocalBackgroundDataService::LocalBackgroundDataService()
    {
        Load();
    }



    const std::vector<BackgroundData>& LocalBackgroundDataService::GetBackgroundDatas() const
    {
        return m_data_vector;
    }



    void LocalBackgroundDataService::Load()
    {
        using namespace std;
        using namespace rapidjson;

        m_data_vector.clear();

        string file_path      = "Data/BackgroundData.json";
        string json_str       = ax::FileUtils::getInstance()->getStringFromFile(file_path);

        Document doc;
        doc.Parse(json_str.c_str());

        const Value& blocks = doc["backgrounds"];
        const Value& bg_array = doc["backgrounds"];

        for (rapidjson::SizeType i = 0; i < bg_array.Size(); i++)
        {
            const rapidjson::Value& item = bg_array[i];
            BackgroundData data;

            data.m_index      = item["index"].GetInt();
            data.m_min_depth  = item["min_depth"].GetInt();
            data.m_frame_name  = item["frame_name"].GetString();
            data.m_tile_pixel = item["tile_pixel"].GetInt();

            m_data_vector.push_back(data);
        }

        std::sort(m_data_vector.begin(),
                  m_data_vector.end(),
                  [](const BackgroundData& a, const BackgroundData& b) { return a.m_min_depth > b.m_min_depth; });
    }
}


