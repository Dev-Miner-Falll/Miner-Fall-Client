#pragma once

#include "axmol/axmol.h"
#include "objectType.h"

class ObjectFactory
{
public:
    static ax::Node* Create(ObjectType object_type);
};
