#pragma once

#include <optional>
#include "Service/ILoadable.h"
#include "Data/Block/BlockData.h"

namespace Service
{
    class IBlockDataService : public ILoadable
    {
    public:
        virtual ~IBlockDataService()                                              = default;

        virtual std::optional<BlockData> GetBlockData(BlockType block_type) const = 0;
    };
}
