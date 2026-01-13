#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Inventory/IInventoryService.h"
#include "Item/IItemDataService.h"
#include "Block/IBlockDataService.h"
#include "Background/IBackgroundDataService.h"
#include "Shop/IShopDataService.h"
#include "Craft/IRecipeDataService.h"
#include "Setting/ISettingService.h"
#include "axmol/axmol.h"

class ServiceLocator
{
public:
    static void Initialize();

    template<typename T>
    static void Register(std::shared_ptr<T> service)
    {
        const std::type_index key(typeid(T));
        auto& services = GetServices();

        if (services.find(key) == services.end())
        {
            AXLOGD("Service Locator: 성공적으로 {}를 등록했습니다.", key.name());
            services[key] = service;
        }
        else
            AXLOGD("Service Locator: 이미 {}가 존재합니다.", key.name());
    }

    template<typename T>
    static std::shared_ptr<T> Get()
    {
        const std::type_index key(typeid(T));
        auto& services = GetServices();

        if (services.find(key) == services.end())
            AXLOGD("Service Locator: {}가 등록되지 않았습니다.", key.name());

        return std::static_pointer_cast<T>(services[key]);
    }

    static void Clear();

    static void Save();

private:
    static std::unordered_map<std::type_index, std::shared_ptr<void>>& GetServices();
};
