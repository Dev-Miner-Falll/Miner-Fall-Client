#pragma once

#include "ExplosiveActionStrategy.h"

class MediumBombStrategy : public ExplosiveItemStrategy
{
public:
    MediumBombStrategy();

    void Execute(const ItemActionContext& context) override;
};
