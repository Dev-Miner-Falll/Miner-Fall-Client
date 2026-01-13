#include "ObjectPoolManager.h"
#include "ObjectFactory.h"
#include "IPoolable.h"

USING_NS_AX;

void ObjectPoolManager::Initialize()
{
    for (const Pool& pool : POOL_TABLE)
        Preload(pool);
}

ax::Node* ObjectPoolManager::Get(ObjectType object_type)
{
    const Pool* pool           = GetPool(object_type);
    RuntimePool& runtime_pool = s_pool_map[object_type];

    Node* object = nullptr;
    if (!runtime_pool.m_pool.empty())
    {
        object = runtime_pool.m_pool.back();
        runtime_pool.m_pool.pop_back();
    }
    else if (pool && pool->m_is_expandable && runtime_pool.m_current_count < pool->m_max_count)
    {
        object = ObjectFactory::Create(object_type);
        runtime_pool.m_current_count++;
    }
    else
    {
        AXLOGD("ObjectPoolManager: 풀이 가득 찼습니다.");
        return nullptr;
    }

    IPoolable* poolable = dynamic_cast<IPoolable*>(object);
    if (poolable)
        poolable->OnEnable();

    return object;
}

void ObjectPoolManager::Release(ax::Node* object)
{
    IPoolable* poolable = dynamic_cast<IPoolable*>(object);
    if (!poolable)
        return;

    poolable->OnDisable();
    object->removeFromParent();

    ObjectType object_type = poolable->GetObjectType();
    s_pool_map.at(object_type).m_pool.push_back(object);
}



void ObjectPoolManager::Preload(const Pool& pool)
{
    RuntimePool& runtime_pool = s_pool_map[pool.m_type];

    for (int i = 0; i < pool.m_initial_count; i++)
    {
        Node* object = ObjectFactory::Create(pool.m_type);
        if (!object)
            return;

        IPoolable* poolable = dynamic_cast<IPoolable*>(object);
        if (poolable)
            poolable->OnDisable();

        runtime_pool.m_pool.push_back(object);
        runtime_pool.m_current_count++;
    }
}

const Pool* ObjectPoolManager::GetPool(ObjectType object_type)
{
    for (const Pool& pool : POOL_TABLE)
        if (pool.m_type == object_type)
            return &pool;

    return nullptr;
}
