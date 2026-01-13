#pragma once

#include "axmol/axmol.h"
#include "Game/Item/ItemActionStrategy.h"

class ExplosiveItemStrategy : public ItemActionStrategy
{
protected:
    float m_radius;
    float m_interval;
    static constexpr float TILE_SIZE = 32.f;

public:
    ExplosiveItemStrategy(float radius, float interval)
        : m_radius(radius)
        , m_interval(interval)
    {}
};
