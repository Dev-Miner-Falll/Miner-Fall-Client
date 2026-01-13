#include "LayerShaker.h"

USING_NS_AX;

bool LayerShaker::init()
{
    if (!Node::init())
        return false;

    schedule(AX_SCHEDULE_SELECTOR(LayerShaker::UpdateShake));

    return true;
}


void LayerShaker::Shake(float strength, float duration)
{
    m_strength = strength;

    m_time = duration;
    m_duration = m_time;

    m_phase = 0.f;
}

Vec2 LayerShaker::GetOffset() const
{
    return m_offset;
}


void LayerShaker::UpdateShake(float delta)
{
    if (m_time <= 0.f)
    {
        m_offset = Vec2::ZERO;
        return;
    }

    m_time -= delta;

    float t    = m_time / m_duration;
    float damp = t * t;

    m_phase += delta * 40.f;

    m_offset.x = std::sin(m_phase) * m_strength * damp;
    m_offset.y = std::sin(m_phase * 1.7f + 1.3f) * m_strength * 0.6f * damp;
}
