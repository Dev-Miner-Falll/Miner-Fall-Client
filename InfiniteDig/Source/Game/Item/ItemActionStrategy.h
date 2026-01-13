#pragma once

#include "ItemActionContext.h"

class ItemActionStrategy
{
public:
    virtual ~ItemActionStrategy() = default;

    virtual void Execute(const ItemActionContext& context)        = 0;
};
