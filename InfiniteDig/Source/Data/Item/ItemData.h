#pragma once

#include <string>
#include "axmol/axmol.h"
#include "ItemCode.h"
#include "ItemType.h"

struct ItemData
{
    ItemCode m_item_code;
    std::string m_item_name;
    std::string m_frame_name;
    ItemType m_item_type;
    float m_cooltime;
};
