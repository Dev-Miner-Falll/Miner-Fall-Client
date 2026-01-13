#include "ObjectFactory.h"
#include "Game/FloatingItem/FloatingItem.h"
#include "Game/Item/Explosive/ExplosiveItem.h"
#include "UI/Craft/IngredientSlot.h"

ax::Node* ObjectFactory::Create(ObjectType object_type)
{
    ax::Node* object = nullptr;

    switch (object_type)
    {
    case ObjectType::FLOATING_ITEM:
        object = FloatingItem::create();
        break;

    case ObjectType::EXPLOSIVE_ITEM:
        object = ExplosiveItem::create();
        break;

    case ObjectType::INGREDIENT_SLOT:
        object = IngredientSlot::create();
        break;

    default:
        return nullptr;
    }

    if (object)
        object->retain();

    return object;
}
