#pragma once

#include "axmol/axmol.h"

class LayerShaker : public ax::Node
{
public:
    CREATE_FUNC(LayerShaker)
    bool init() override;

public:
    void Shake(float strength, float duration);
    ax::Vec2 GetOffset() const;

private:
    void UpdateShake(float delta);

private:
    float m_time;
    float m_duration;
    float m_strength;
    float m_phase;

    ax::Vec2 m_offset;
};
