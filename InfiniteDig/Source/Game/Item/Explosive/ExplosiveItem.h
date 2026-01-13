#pragma once

#include "axmol/axmol.h"
#include "Utils/Pool/IPoolable.h"
#include "Game/Item/ItemActionContext.h"

class ExplosiveItem : public ax::Sprite, public IPoolable
{
public:
    CREATE_FUNC(ExplosiveItem)
    bool init() override;
    void onEnter() override;

public:
    void Inject(std::string frame_name, ax::Vec2 position, float radius, float delay);
    void Activate(const ItemActionContext& action_context, float shaking_strength, float shaking_duration);

public:
    void OnEnable() override;
    void OnDisable() override;
    ObjectType GetObjectType() const;

private:
    void CreateBlinkAction();
    void CreateExplosion(const ItemActionContext& action_context, float shaking_strength, float shaking_duration);

private:
    float m_delay;
    float m_radius;
    ax::PhysicsBody* m_body;
};
