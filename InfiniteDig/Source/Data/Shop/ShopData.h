#pragma once

#include <string>
#include "Data/Item/ItemData.h"

struct ShopData
{
    ItemCode m_item_code;

    std::string m_item_name;
    std::string m_item_desc;

    int m_item_cost;
};
