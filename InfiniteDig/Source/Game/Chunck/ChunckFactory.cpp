#include "ChunckFactory.h"
#include "axmol/axmol.h"

ChunckData ChunckFactory::Create(int start_depth, const BlockSelector& selector)
{
    ChunckData new_chunck;

    for (int y = 0; y < ChunckData::HEIGHT; y++)
    {
        int depth = start_depth + y;
        for (int x = 0; x < ChunckData::WIDTH; x++)
        {
            if (x == 0 || x == ChunckData::WIDTH - 1)
                new_chunck.m_block_array[y][x] = BlockType::BEDROCK;
            else
                new_chunck.m_block_array[y][x] = selector.Select(depth);
        }
    }

    return new_chunck;
}
