#include "ServiceLocator.h"
#include "Inventory/LocalInventoryService.h"
#include "Item/LocalItemDataService.h"
#include "Shop/LocalShopDataService.h"
#include "Block/LocalBlockDataService.h"
#include "Background/LocalBackgroundDataService.h"
#include "Setting/LocalSettingService.h"
#include "Craft/LocalRecipeDataService.h"

void ServiceLocator::Initialize()
{
    using namespace std;
    using namespace Service;

    Register<IInventoryService>(make_shared<LocalInventoryService>());
    Register<IItemDataService>(make_shared<LocalItemDataService>());
    Register<IShopDataService>(make_shared<LocalShopDataService>());
    Register<IRecipeDataService>(make_shared<LocalRecipeDataService>());
    Register<IBlockDataService>(make_shared<LocalBlockDataService>());
    Register<IBackgroundDataService>(make_shared<LocalBackgroundDataService>());
    Register<ISettingService>(make_shared<LocalSettingService>());
}

void ServiceLocator::Clear()
{
    GetServices().clear();
    AXLOGD("Service Locator: 성공적으로 서비스들을 모두 해제했습니다.");
}

std::unordered_map<std::type_index, std::shared_ptr<void>>& ServiceLocator::GetServices()
{
    static std::unordered_map<std::type_index, std::shared_ptr<void>> services;
    return services;
}

void ServiceLocator::Save() 
{
    using namespace Service;

    Get<IInventoryService>()->Save();
    Get<ISettingService>()->Save();
}
