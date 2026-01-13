#pragma once

#include "Data/Block/ChunckData.h"
#include "Game/Chunck/BlockSelector.h"

class ChunckFactory
{
public:
    static ChunckData Create(int start_depth, const BlockSelector& selector);
};
