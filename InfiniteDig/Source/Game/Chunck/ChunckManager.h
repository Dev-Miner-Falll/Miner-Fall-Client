#pragma once

#include <vector>
#include "axmol/axmol.h"
#include "Game/Chunck/Chunck.h"
#include "Game/Chunck/BlockSelector.h"
#include "Service/Inventory/IInventoryService.h"
#include "Game/BackgroundManager.h"

class ChunckManager : public ax::Node
{
public:
    static ChunckManager* create(std::shared_ptr<Service::IBlockDataService> block_data_service,
                                 std::shared_ptr<Service::IInventoryService> inventory_service,
                                 BlockSelector* block_selector,
                                 BackgroundManager* background_manager);

    bool init(std::shared_ptr<Service::IBlockDataService> block_data_service,
              std::shared_ptr<Service::IInventoryService> inventory_service,
              BlockSelector* block_selector,
              BackgroundManager* background_manager,
              int start_depth);

public:
    void UpdatePickaxe(const ax::Vec2& pickaxe_pos, float pickaxe_radius, float delta);
    std::vector<Block*> GetBlocksCollidedWithPickaxe(const ax::Vec2& pickaxe_pos, float pickaxe_radius);

private:
    void Rebuild();
    void HandleBlockBreak(int depth, ItemCode item_code);
    
    bool CircleRectCollision(const ax::Vec2& circle_center, float radius, const ax::Rect& rect);

private:
    std::shared_ptr<Service::IInventoryService> m_inventory_service;
    BlockSelector* m_selector;
    BackgroundManager* m_background_manager;

    std::vector<Chunck*> m_chunck_vector;

    int m_deepest_world_y_index;
    int m_current_chunck_index;
    int m_bottm_depth;
};
