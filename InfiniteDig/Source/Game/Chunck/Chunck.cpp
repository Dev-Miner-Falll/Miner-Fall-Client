#include "Chunck.h"

USING_NS_AX;

Chunck* Chunck::create(std::shared_ptr<Service::IBlockDataService> block_data_service)
{
    Chunck* new_chunck = new (std::nothrow) Chunck();
    if (new_chunck && new_chunck->init(block_data_service))
    {
        new_chunck->autorelease();
        return new_chunck;
    }

    AX_SAFE_DELETE(new_chunck);
    return nullptr;
}

bool Chunck::init(std::shared_ptr<Service::IBlockDataService> block_data_service)
{
    if (!Node::init())
        return false;

    m_block_data_service = block_data_service;

    CreateBlocks();
    this->setContentSize(Size(ChunckData::WIDTH * TILE_SIZE, ChunckData::HEIGHT * TILE_SIZE));

    return true;
}


void Chunck::CreateBlocks()
{
    for (int y = 0; y < ChunckData::HEIGHT; y++)
    {
        for (int x = 0; x < ChunckData::WIDTH; x++)
        {
            Block* block = Block::create();
            block->setAnchorPoint(Vec2::ZERO);
            block->setPosition(Vec2(x * TILE_SIZE, (ChunckData::HEIGHT - 1 - y) * TILE_SIZE));
            this->addChild(block);

            m_block_array[y][x] = block;
        }
    }
}

void Chunck::Rebuild(const ChunckData& chunck_data, int start_y_index, std::function<void(int, ItemCode)> on_block_break)
{
    for (int y = 0; y < ChunckData::HEIGHT; y++)
    {
        for (int x = 0; x < ChunckData::WIDTH; x++)
        {
            BlockType type = chunck_data.m_block_array[y][x];

            std::optional<BlockData> optional_block_data = m_block_data_service->GetBlockData(type);
            if (!optional_block_data.has_value())
            {
                m_block_array[y][x]->setVisible(false);
                continue;
            }

            int world_y = start_y_index + y;

            m_block_array[y][x]->setVisible(true);
            m_block_array[y][x]->Inject(optional_block_data.value(), world_y);
            m_block_array[y][x]->setBreakCallback(on_block_break);
        }
    }
}

Block* Chunck::GetBlock(ax::Vec2 position)
{
    if (position.x < 0 || position.x >= ChunckData::WIDTH || position.y < 0 || position.y >= ChunckData::HEIGHT)
        return nullptr;

    return m_block_array[position.y][position.x];
}
