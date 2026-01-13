#pragma once

#include <vector>
#include <optional>
#include "Service/ILoadable.h"
#include "Data/Item/ItemData.h"

namespace Service
{
    class IItemDataService : public ILoadable
    {
    public:
        virtual ~IItemDataService()                                           = default;

        virtual std::optional<ItemData> GetItemData(ItemCode item_code) const = 0;
        virtual const std::vector<ItemData>& GetStuffs() const                = 0;
        virtual const std::vector<ItemData>& GetRefines() const               = 0;
        virtual const std::vector<ItemData>& GetItems() const                 = 0;
    };
}

