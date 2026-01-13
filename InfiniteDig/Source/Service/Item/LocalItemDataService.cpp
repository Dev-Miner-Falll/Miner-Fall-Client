#include "LocalItemDataService.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "axmol/axmol.h"

namespace Service
{
    LocalItemDataService::LocalItemDataService()
    {
        Load();
    }



    std::optional<ItemData> LocalItemDataService::GetItemData(ItemCode item_code) const
    {
        if (!IsExist(item_code))
            return std::nullopt;

        return m_item_map.at(item_code);
    }

    const std::vector<ItemData>& LocalItemDataService::GetStuffs() const
    {
        return m_stuff_vector;
    }

    const std::vector<ItemData>& LocalItemDataService::GetRefines() const
    {
        return m_refines_vector;
    }

    const std::vector<ItemData>& LocalItemDataService::GetItems() const
    {
        return m_items_vector;
    }



    void LocalItemDataService::Load()
    {
        using namespace rapidjson;

        m_item_map.clear();

        std::string file_path = ax::FileUtils::getInstance()->getWritablePath() + "ItemData.json";
        std::string json_str  = ax::FileUtils::getInstance()->getStringFromFile(file_path);
        AXASSERT(!json_str.empty(), "ItemData.json을 불러오는 데 실패했습니다.");

        Document doc;
        doc.Parse(json_str.c_str());

        AXASSERT(doc.IsObject(), "ItemData.json의 형식이 잘못되었습니다.");
        AXASSERT(doc.HasMember("items") && doc["items"].IsArray(), "ItemData.json은 배열이 아닙니다.");

        const auto& items = doc["items"];

        for (const auto& v : items.GetArray())
        {
            ItemData data{};

            data.m_item_code = static_cast<ItemCode>(v["item_code"].GetInt());
            data.m_item_name = v["item_name"].GetString();
            data.m_frame_name = v["frame_name"].GetString();
            data.m_item_type = static_cast<ItemType>(v["item_type"].GetInt());
            data.m_cooltime   = v["item_cool"].GetFloat();

            m_item_map.emplace(data.m_item_code, data);


            switch (data.m_item_type)
            {
            case ItemType::STUFF:
                m_stuff_vector.push_back(data);
                break;

            case ItemType::REFINE:
                m_refines_vector.push_back(data);
                break;

            case ItemType::ITEM:
                m_items_vector.push_back(data);
                break;

            default:
                break;
            }
        }
    }



    bool LocalItemDataService::IsExist(ItemCode item_code) const
    {
        if (m_item_map.find(item_code) == m_item_map.end())
            return false;

        return true;
    }
}

