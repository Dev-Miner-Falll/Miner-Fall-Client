#pragma once

#include <array>
#include "Data/Block/BlockType.h"

struct ChunckData
{
    static constexpr int WIDTH  = 12;
    static constexpr int HEIGHT = 12;

    std::array<std::array<BlockType, WIDTH>, HEIGHT> m_block_array;
};
