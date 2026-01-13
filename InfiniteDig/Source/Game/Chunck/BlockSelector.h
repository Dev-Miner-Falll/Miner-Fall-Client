#pragma once

#include "axmol/axmol.h"
#include "Service/Block/IBlockDataService.h"

class BlockSelector : public ax::Node
{
public:
    static BlockSelector* create(std::shared_ptr<Service::IBlockDataService> block_data_service);
    virtual bool init(std::shared_ptr<Service::IBlockDataService> block_data_service);

public:
    BlockType Select(int depth) const;

private:
    std::shared_ptr<Service::IBlockDataService> m_block_data_service;
};
