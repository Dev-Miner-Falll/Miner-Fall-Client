#pragma once

#include "Service/ISaveable.h"
#include "Data/Inventory/InventoryData.h"

namespace Service
{
    class IInventoryService : public ISaveable
    {
    public:
        virtual ~IInventoryService()                                 = default;

        virtual void Initialize()                                    = 0;
        virtual void IncreaseItem(ItemCode item_code, int count = 1) = 0;
        virtual void DecreaseItem(ItemCode item_code, int count = 1) = 0;
        virtual int GetItemCount(ItemCode item_code) const           = 0;
    };
}
