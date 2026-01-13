#pragma once

#include "ObjectType.h"

class IPoolable
{
public:
    virtual ~IPoolable()                     = default;

    virtual void OnEnable()                  = 0;
    virtual void OnDisable()                 = 0;
    virtual ObjectType GetObjectType() const = 0;
};
