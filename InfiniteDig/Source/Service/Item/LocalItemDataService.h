#pragma once

#include <map>
#include "IItemDataService.h"

namespace Service
{
    class LocalItemDataService : public IItemDataService
    {
    public:
        std::optional<ItemData> GetItemData(ItemCode item_code) const override;
        const std::vector<ItemData>& GetStuffs() const override;
        const std::vector<ItemData>& GetRefines() const override;
        const std::vector<ItemData>& GetItems() const override;

    public:
        void Load() override;

    public:
        LocalItemDataService();

        LocalItemDataService(const LocalItemDataService&)            = delete;
        LocalItemDataService& operator=(const LocalItemDataService&) = delete;

    private:
        bool IsExist(ItemCode item_code) const;

    private:
        std::map<ItemCode, ItemData> m_item_map;
        std::vector<ItemData> m_stuff_vector;
        std::vector<ItemData> m_refines_vector;
        std::vector<ItemData> m_items_vector;
    };
}
