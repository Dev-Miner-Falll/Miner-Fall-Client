#include <iostream>
#include <string>
#include "axmol/axmol.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "LocalBlockDataService.h"

namespace Service
{
    LocalBlockDataService::LocalBlockDataService()
    {
        Load();
    }



    std::optional<BlockData> LocalBlockDataService::GetBlockData(BlockType block_type) const
    {
        if (!IsExist(block_type))
            return std::nullopt;

        return m_block_map.at(block_type);
    }



    void LocalBlockDataService::Load()
    {
        using namespace std;
        using namespace rapidjson;

        m_block_map.clear();

        string file_path = ax::FileUtils::getInstance()->getWritablePath() + "BlockData.json";
        string json_str  = ax::FileUtils::getInstance()->getStringFromFile(file_path);

        AXASSERT(!json_str.empty(), "BlockData.json 로드에 실패했습니다.");

        Document doc;
        doc.Parse(json_str.c_str());

        AXASSERT(doc.IsObject(), "BlockData.json의 형식이 잘못되었습니다.");
        AXASSERT(doc.HasMember("blocks"), "BlockData.json의 형식이 잘못되었습니다.");

        const Value& blocks = doc["blocks"];
        AXASSERT(blocks.IsArray(), "blocks는 배열이 아닙니다.");

        for (const auto& block : blocks.GetArray())
        {
            BlockData data;

            data.m_block_type = static_cast<BlockType>(block["block_type"].GetInt());
            data.m_item_code  = static_cast<ItemCode>(block["item_code"].GetInt());
            data.m_frame_name = block["frame_name"].GetString();
            data.m_item_frame_name = block["item_frame_name"].GetString();
            data.m_break_time = block["break_time"].GetFloat();
            data.m_min_depth = block["min_depth"].GetInt();
            data.m_max_depth = block["max_depth"].GetInt();
            data.m_weight       = block["weight"].GetInt();
            data.m_is_breakable = block["breakable"].GetBool();

            m_block_map.emplace(data.m_block_type, data);
        }
    }



    bool LocalBlockDataService::IsExist(BlockType block_type) const
    {
        if (m_block_map.find(block_type) == m_block_map.end())
            return false;

        return true;
    }
}
