#pragma once

#include "ExplosiveActionStrategy.h"

class MediumDynamiteStrategy : public ExplosiveItemStrategy
{
public:
    MediumDynamiteStrategy();

    void Execute(const ItemActionContext& context) override;
};
