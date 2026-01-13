#pragma once

#include "ObjectType.h"

struct Pool
{
    ObjectType m_type;
    int m_initial_count;
    int m_max_count;
    bool m_is_expandable;
};

static const Pool POOL_TABLE[] =
{
    { ObjectType::FLOATING_ITEM, 10, 20, true },
    { ObjectType::EXPLOSIVE_ITEM, 4, 6, true },
    { ObjectType::INGREDIENT_SLOT, 1, 5, true },
};
