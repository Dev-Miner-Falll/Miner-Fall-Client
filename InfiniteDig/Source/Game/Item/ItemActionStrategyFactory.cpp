#include "ItemActionStrategyFactory.h"
#include "Explosive/SmallDynamiteStrategy.h"
#include "Explosive/MediumDynamiteStrategy.h"
#include "Explosive/MediumBombStrategy.h"
#include "Explosive/ExtraBombStrategy.h"

std::unique_ptr<ItemActionStrategy> ItemActionStrategyFactory::Create(ItemCode code)
{
    switch (code)
    {
    case ItemCode::SMALL_DYNAMITE:
        return std::make_unique<SmallDynamiteStrategy>();

    case ItemCode::MEDIUM_DYNAMITE:
        return std::make_unique<MediumDynamiteStrategy>();

    case ItemCode::MEDIUM_BOMB:
        return std::make_unique<MediumBombStrategy>();

    case ItemCode::EXTRA_BOMB:
        return std::make_unique<ExtraBombStrategy>();

    default:
        return nullptr;
    }
}
