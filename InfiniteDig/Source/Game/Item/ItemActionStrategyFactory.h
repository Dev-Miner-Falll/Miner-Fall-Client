#pragma once

#include <memory>
#include "ItemActionStrategy.h"
#include "Data/Item/ItemCode.h"

class ItemActionStrategyFactory
{
public:
    static std::unique_ptr<ItemActionStrategy> Create(ItemCode code);
};
