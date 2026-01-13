#pragma once

#include <map>
#include "IBlockDataService.h"

namespace Service
{
    class LocalBlockDataService : public IBlockDataService
    {
    public:
        std::optional<BlockData> GetBlockData(BlockType block_type) const override;

    public:
        void Load() override;

    public:
        LocalBlockDataService();

        LocalBlockDataService(const LocalBlockDataService&) = delete;
        LocalBlockDataService& operator=(const LocalBlockDataService&) = delete;

    private:
        bool IsExist(BlockType block_type) const;

    private:
        std::map<BlockType, BlockData> m_block_map;
    };
}
