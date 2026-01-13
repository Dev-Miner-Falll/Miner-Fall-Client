#pragma once

#include <string>
#include "axmol/axmol.h"
#include "BlockType.h"
#include "Data/Item/ItemCode.h"

struct BlockData
{
    BlockType m_block_type;
    ItemCode m_item_code;

    std::string m_frame_name;
    std::string m_item_frame_name;

    float m_break_time;
    int m_min_depth;
    int m_max_depth;
    int m_weight;
    bool m_is_breakable;
};
