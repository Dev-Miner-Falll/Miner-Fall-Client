#include "ChunckManager.h"
#include "Game/Chunck/ChunckFactory.h"

USING_NS_AX;

ChunckManager* ChunckManager::create(std::shared_ptr<Service::IBlockDataService> block_data_service,
                                     std::shared_ptr<Service::IInventoryService> inventory_service,
                                     BlockSelector* block_selector,
                                     BackgroundManager* background_manager)
{
    ChunckManager* chunck_manager = new (std::nothrow) ChunckManager();
    if (chunck_manager &&
        chunck_manager->init(block_data_service, inventory_service, block_selector, background_manager, 0))
    {
        chunck_manager->autorelease();
        return chunck_manager;
    }

    AX_SAFE_DELETE(chunck_manager);
    return nullptr;
}

bool ChunckManager::init(std::shared_ptr<Service::IBlockDataService> block_data_service,
                         std::shared_ptr<Service::IInventoryService> inventory_service,
                         BlockSelector* block_selector,
                         BackgroundManager* background_manager,
                         int start_depth)
{
    if (!Node::init())
        return false;

    m_inventory_service = inventory_service;
    m_background_manager = background_manager;
    m_selector = block_selector;

    constexpr int VISIBLE_CHUNCK_COUNT = 4;

    m_current_chunck_index = start_depth / ChunckData::HEIGHT;
    m_bottm_depth          = start_depth;

    m_background_manager->ReplaceBackground(start_depth);

    for (int i = 0; i < VISIBLE_CHUNCK_COUNT; i++)
    {
        int chunck_start_depth = start_depth + i * ChunckData::HEIGHT;

        ChunckData chunck_data = ChunckFactory::Create(chunck_start_depth, *m_selector);

        auto callback = [this](int y, ItemCode item_code) { this->HandleBlockBreak(y, item_code); };
        Vec2 chunck_position(-11.f, 0.f - i * ChunckData::HEIGHT * 32.f);
        Chunck* chunck = Chunck::create(block_data_service);
        chunck->Rebuild(chunck_data, chunck_start_depth, callback);
        chunck->setPosition(chunck_position);
        this->addChild(chunck);
        m_chunck_vector.push_back(chunck);

        m_bottm_depth = chunck_start_depth;
    }

    return true;
}



void ChunckManager::Rebuild()
{
    Chunck* top = m_chunck_vector.front();
    m_chunck_vector.erase(m_chunck_vector.begin());

    Chunck* last_chunck       = m_chunck_vector.back();
    float last_y              = last_chunck->getPositionY();
    float chunck_pixel_height = ChunckData::HEIGHT * 32.0f;

    float new_y   = last_y - chunck_pixel_height;
    int new_depth = m_bottm_depth + ChunckData::HEIGHT;

    auto callback       = [this](int y, ItemCode item_code) { this->HandleBlockBreak(y, item_code); };
    ChunckData new_data = ChunckFactory::Create(new_depth, *m_selector);
    top->Rebuild(new_data, new_depth, callback);
    top->setPositionY(new_y);

    m_chunck_vector.push_back(top);
    m_bottm_depth = new_depth;

    m_bottm_depth = new_depth;
}

void ChunckManager::HandleBlockBreak(int depth, ItemCode item_code)
{
    m_inventory_service->IncreaseItem(item_code);

    Chunck* top             = m_chunck_vector.front();
    Vec2 top_world_position = this->convertToWorldSpace(top->getPosition());

    float screen_height = Director::getInstance()->getVisibleSize().height;
    if (top_world_position.y > screen_height + 320.0f)
        Rebuild();
}



void ChunckManager::UpdatePickaxe(const ax::Vec2& pickaxe_pos, float pickaxe_radius, float delta)
{
    std::vector<Block*> blocks = GetBlocksCollidedWithPickaxe(pickaxe_pos, pickaxe_radius);

    for (auto* block : blocks)
        block->StartMining(delta);
}

std::vector<Block*> ChunckManager::GetBlocksCollidedWithPickaxe(const ax::Vec2& pickaxe_pos, float pickaxe_radius)
{
    std::vector<Block*> collided_block_vector;

    Vec2 pickaxe_world_position = m_selector->getParent()->convertToWorldSpace(pickaxe_pos);

    for (auto* chunck : m_chunck_vector)
    {
        for (int y = 0; y < ChunckData::HEIGHT; y++)
        {
            for (int x = 0; x < ChunckData::WIDTH; x++)
            {
                Block* block = chunck->GetBlock(Vec2(x, y));
                if (!block || block->IsBroken() || !block->isVisible())
                    continue;

                auto bb = block->getBoundingBox();
                Vec2 worldMin = block->getParent()->convertToWorldSpace(bb.origin);
                Vec2 worldMax = block->getParent()->convertToWorldSpace(Vec2(bb.getMaxX(), bb.getMaxY()));

                Rect world_rect = Rect(worldMin.x, worldMin.y, worldMax.x - worldMin.x, worldMax.y - worldMin.y);

                if (CircleRectCollision(pickaxe_world_position, pickaxe_radius, world_rect))
                {
                    collided_block_vector.push_back(block);

                    m_deepest_world_y_index = std::max(m_deepest_world_y_index, block->GetWorldYIndex());

                    if (m_background_manager)
                        m_background_manager->ReplaceBackground(m_deepest_world_y_index);
                }
            }
        }
    }
    return collided_block_vector;
}

bool ChunckManager::CircleRectCollision(const ax::Vec2& circle_center, float radius, const ax::Rect& rect)
{
    float closestX = std::max(rect.getMinX(), std::min(circle_center.x, rect.getMaxX()));
    float closestY = std::max(rect.getMinY(), std::min(circle_center.y, rect.getMaxY()));

    float dx = circle_center.x - closestX;
    float dy = circle_center.y - closestY;

    return (dx * dx + dy * dy) <= (radius * radius);
}
