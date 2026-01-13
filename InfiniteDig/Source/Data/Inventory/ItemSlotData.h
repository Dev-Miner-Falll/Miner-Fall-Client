#pragma once

#include "Data/Item/ItemCode.h"

struct ItemSlotData
{
    ItemCode m_item_code;
    int m_item_count;

    ItemSlotData(ItemCode item_code = ItemCode::NONE, int item_count = 0)
        : m_item_code(item_code)
        , m_item_count(item_count)
    {}
};
