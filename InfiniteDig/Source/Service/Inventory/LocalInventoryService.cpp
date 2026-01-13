#include <climits>
#include <algorithm>
#include <string>
#include "axmol/axmol.h"
#include "LocalInventoryService.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace Service
{
    LocalInventoryService::LocalInventoryService()
    {
        Load();
    }



    void LocalInventoryService::Initialize()
    {
        USING_NS_AX;

        EventCustom event("INVENTORY_UPDATE");

        for (const auto& pair : m_inventory_map)
        {
            ItemSlotData slot_data = ItemSlotData(pair.first, pair.second);
            event.setUserData(&slot_data);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }

    void LocalInventoryService::IncreaseItem(ItemCode item_code, int count)
    {
        USING_NS_AX;

        if (item_code == ItemCode::NONE || count <= 0)
            return;

        if (!IsExist(item_code))
            return;

        m_inventory_map.at(item_code) += count;
        m_inventory_map.at(item_code) = std::clamp(m_inventory_map.at(item_code), 0, INT_MAX);

        ItemSlotData slot_data = ItemSlotData(item_code, m_inventory_map.at(item_code));
        EventCustom event("INVENTORY_UPDATE");

        event.setUserData(&slot_data);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }

    void LocalInventoryService::DecreaseItem(ItemCode item_code, int count)
    {
        USING_NS_AX;

        if (item_code == ItemCode::NONE || count <= 0)
            return;

        if (!IsExist(item_code))
            return;

        m_inventory_map.at(item_code) -= count;
        m_inventory_map.at(item_code) = std::clamp(m_inventory_map.at(item_code), 0, INT_MAX);

        ItemSlotData slot_data = ItemSlotData(item_code, m_inventory_map.at(item_code));
        EventCustom event("INVENTORY_UPDATE");

        event.setUserData(&slot_data);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }

    int LocalInventoryService::GetItemCount(ItemCode item_code) const
    {
        if (!IsExist(item_code))
            return -1;

        return m_inventory_map.at(item_code);
    }



    void LocalInventoryService::Load()
    {
        using namespace rapidjson;
        using namespace std;

        string file_path = ax::FileUtils::getInstance()->getWritablePath() + "InventoryData.json";
        if (!ax::FileUtils::getInstance()->isFileExist(file_path))
        {
            InitializeData();
            return;
        }

        string json = ax::FileUtils::getInstance()->getStringFromFile(file_path);

        Document document;
        document.Parse(json.c_str());
        if (!document.IsObject() || !document.HasMember("Inventory") || !document["Inventory"].IsArray())
        {
            InitializeData();
            return;
        }

        InventoryData inventory_data;
        inventory_data.m_item_vector.clear();

        const Value& inventory_array = document["Inventory"];
        for (SizeType i = 0; i < inventory_array.Size(); i++)
        {
            const Value& item = inventory_array[i];
            if (!item.IsObject() || !item.HasMember("ItemCode") || !item.HasMember("Count"))
                continue;

            ItemCode item_code = static_cast<ItemCode>(item["ItemCode"].GetInt());
            int count          = item["Count"].GetInt();

            inventory_data.m_item_vector.emplace_back(item_code, count);
        }

        if (inventory_data.m_item_vector.empty())
        {
            InitializeData();
            return;
        }

        m_inventory_map.clear();
        for (const ItemSlotData& item_data : inventory_data.m_item_vector)
            m_inventory_map[item_data.m_item_code] = item_data.m_item_count;
    }

    void LocalInventoryService::Save()
    {
        using namespace rapidjson;
        using namespace std;

        InventoryData inventory_data;
        inventory_data.m_item_vector.clear();

        for (const auto& pair : m_inventory_map)
            inventory_data.m_item_vector.emplace_back(pair.first, pair.second);

        Document document;
        document.SetObject();
        Document::AllocatorType& allocator = document.GetAllocator();

        Value inventory_array(kArrayType);

        for (const ItemSlotData& item_data : inventory_data.m_item_vector)
        {
            Value item_obj(kObjectType);
            item_obj.AddMember("ItemCode", static_cast<int>(item_data.m_item_code), allocator);
            item_obj.AddMember("Count", item_data.m_item_count, allocator);

            inventory_array.PushBack(item_obj, allocator);
        }

        document.AddMember("Inventory", inventory_array, allocator);

        StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);
        document.Accept(writer);

        string file_path = ax::FileUtils::getInstance()->getWritablePath() + "InventoryData.json";
        ax::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), file_path);
    }


    void LocalInventoryService::InitializeData()
    {
        InventoryData inventory_data;
        
        m_inventory_map.clear();
        for (const ItemSlotData& item_data : inventory_data.m_item_vector)
            m_inventory_map.insert(std::make_pair(item_data.m_item_code, item_data.m_item_count));

        Save();
    }

    bool LocalInventoryService::IsExist(ItemCode item_code) const
    {
        return m_inventory_map.find(item_code) != m_inventory_map.end();
    }
}
