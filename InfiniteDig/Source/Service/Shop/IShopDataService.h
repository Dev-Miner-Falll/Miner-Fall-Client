#pragma once

#include <vector.h>
#include <optional>
#include "Service/ILoadable.h"
#include "Data/Shop/ShopData.h"

namespace Service
{
    class IShopDataService : public ILoadable
    {
    public:
        virtual ~IShopDataService()                                           = default;

        virtual std::optional<ShopData> GetShopData(ItemCode item_code) const = 0;
        virtual const std::vector<ShopData>& GetAllShopDatas() const          = 0;
    };
}
