#pragma once

#include <unordered_map>
#include "axmol/axmol.h"
#include "Pool.h"
#include "RuntimePool.h"

class ObjectPoolManager
{
public:
    static void Initialize();
    static ax::Node* Get(ObjectType object_type);
    static void Release(ax::Node* object);

private:
    static void Preload(const Pool& pool);
    static const Pool* GetPool(ObjectType object_type);

private:
    inline static std::unordered_map<ObjectType, RuntimePool> s_pool_map;
};
