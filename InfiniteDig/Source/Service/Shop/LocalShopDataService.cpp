#include "LocalShopDataService.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "axmol/axmol.h"

namespace Service
{
    LocalShopDataService::LocalShopDataService()
    {
        Load();
    }


    std::optional<ShopData> LocalShopDataService::GetShopData(ItemCode item_code) const
    {
        if (!IsExist(item_code))
            return std::nullopt;

        return m_item_map.at(item_code);
    }

    const std::vector<ShopData>& LocalShopDataService::GetAllShopDatas() const
    {
        return m_item_vector;
    }



    void LocalShopDataService::Load()
    {
        using namespace rapidjson;

        m_item_map.clear();

        std::string file_path = "Data/ShopData.json";
        std::string json_str  = ax::FileUtils::getInstance()->getStringFromFile(file_path);

        Document doc;
        doc.Parse(json_str.c_str());

        const auto& items = doc["items"];

        for (const auto& v : items.GetArray())
        {
            ShopData data{};

            data.m_item_code = static_cast<ItemCode>(v["item_code"].GetInt());
            data.m_item_name = v["item_name"].GetString();
            data.m_item_desc = v["item_desc"].GetString();
            data.m_item_cost = v["item_cost"].GetInt();

            m_item_map.emplace(data.m_item_code, data);
            m_item_vector.push_back(data);
        }
    }



    bool LocalShopDataService::IsExist(ItemCode item_code) const
    {
        if (m_item_map.find(item_code) == m_item_map.end())
            return false;

        return true;
    }
}
