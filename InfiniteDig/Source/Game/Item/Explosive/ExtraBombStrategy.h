#pragma once

#include "ExplosiveActionStrategy.h"

class ExtraBombStrategy : public ExplosiveItemStrategy
{
public:
    ExtraBombStrategy();

    void Execute(const ItemActionContext& context) override;
};
