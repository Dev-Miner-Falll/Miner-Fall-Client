#pragma once

#include <array>
#include "axmol/axmol.h"
#include "Service/Block/IBlockDataService.h"
#include "Data/Block/ChunckData.h"
#include "BlockSelector.h"
#include "Block.h"

class Chunck : public ax::Node
{
public:
    static Chunck* create(std::shared_ptr<Service::IBlockDataService> block_data_service);
    virtual bool init(std::shared_ptr<Service::IBlockDataService> block_data_service);

public:
    void CreateBlocks();
    void Rebuild(const ChunckData& chunck_data, int start_y_index, std::function<void(int, ItemCode)> on_block_break);
    Block* GetBlock(ax::Vec2 position);

private:
    static constexpr float TILE_SIZE = 32.f;

private:
    std::shared_ptr<Service::IBlockDataService> m_block_data_service;
    std::array<std::array<Block*, ChunckData::WIDTH>, ChunckData::HEIGHT> m_block_array;
};
