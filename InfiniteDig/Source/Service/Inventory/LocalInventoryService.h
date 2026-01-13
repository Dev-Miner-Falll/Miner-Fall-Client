#pragma once

#include <map>
#include "IInventoryService.h"

namespace Service
{
    class LocalInventoryService : public IInventoryService
    {
    public:
        void Initialize() override;
        void IncreaseItem(ItemCode item_code, int count) override;
        void DecreaseItem(ItemCode item_code, int count) override;
        int GetItemCount(ItemCode item_code) const;

    public:
        void Load() override;
        void Save() override;

    public:
        LocalInventoryService();

        LocalInventoryService(const LocalInventoryService&) = delete;
        LocalInventoryService& operator=(const LocalInventoryService&) = delete;

    private:
        bool IsExist(ItemCode item_code) const;
        void InitializeData();

    private:
        std::map<ItemCode, int> m_inventory_map;
    };
}
