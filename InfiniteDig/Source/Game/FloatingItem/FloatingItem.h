#pragma once

#include "axmol/axmol.h"
#include "Utils/Pool/IPoolable.h"

class FloatingItem : public ax::Sprite, public IPoolable
{
public:
    CREATE_FUNC(FloatingItem)
    bool init() override;

public:
    void OnEnable() override;
    void OnDisable() override;
    ObjectType GetObjectType() const override;

public:
    void SetFrame(std::string frame_name);
    void PlayFloatingAnimation();
};
