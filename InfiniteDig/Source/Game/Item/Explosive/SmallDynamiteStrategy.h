#pragma once

#include "ExplosiveActionStrategy.h"

class SmallDynamiteStrategy : public ExplosiveItemStrategy
{
public:
    SmallDynamiteStrategy();

    void Execute(const ItemActionContext& context) override;
};
