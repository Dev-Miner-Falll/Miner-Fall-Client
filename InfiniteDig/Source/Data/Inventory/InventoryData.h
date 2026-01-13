#pragma once

#include "vector.h"
#include "ItemSlotData.h"

struct InventoryData
{
    std::vector<ItemSlotData> m_item_vector;

    InventoryData()
    {
        m_item_vector.reserve(std::size(ALL_ITEM_CODES));

        for (ItemCode item_code : ALL_ITEM_CODES)
            m_item_vector.emplace_back(item_code, 0);
    }

    InventoryData(const std::vector<ItemSlotData>& item_vector)
        : m_item_vector(item_vector)
    {}
};
