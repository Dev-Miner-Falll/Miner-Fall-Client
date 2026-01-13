#pragma once

#include <map>
#include "IShopDataService.h"

namespace Service
{
    class LocalShopDataService : public IShopDataService
    {
    public:
        std::optional<ShopData> GetShopData(ItemCode item_code) const override;
        const std::vector<ShopData>& GetAllShopDatas() const override;

    public:
        void Load() override;

    public:
        LocalShopDataService();

        LocalShopDataService(const LocalShopDataService&) = delete;
        LocalShopDataService& operator=(const LocalShopDataService&) = delete;

    private:
        bool IsExist(ItemCode item_code) const;

    private:
        std::map<ItemCode, ShopData> m_item_map;
        std::vector<ShopData> m_item_vector;
    };
}
