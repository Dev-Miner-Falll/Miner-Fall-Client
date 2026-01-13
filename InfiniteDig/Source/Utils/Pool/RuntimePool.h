#pragma once

#include <vector>
#include "axmol/axmol.h"

struct RuntimePool
{
    std::vector<ax::Node*> m_pool;
    int m_current_count;
};
