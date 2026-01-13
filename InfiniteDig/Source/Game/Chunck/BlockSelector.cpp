#include <vector>
#include "BlockSelector.h"

using namespace std;
USING_NS_AX;

BlockSelector* BlockSelector::create(std::shared_ptr<Service::IBlockDataService> block_data_service)
{
    BlockSelector* block_selector = new (std::nothrow) BlockSelector();
    if (block_selector && block_selector->init(block_data_service))
    {
        block_selector->autorelease();
        return block_selector;
    }

    AX_SAFE_DELETE(block_selector);
    return nullptr;
}

bool BlockSelector::init(std::shared_ptr<Service::IBlockDataService> block_data_service)
{
    if (!Node::init())
        return false;

    m_block_data_service = block_data_service;

    return true;
}



BlockType BlockSelector::Select(int depth) const
{
    vector<pair<BlockType, int>> candidate_vector;
    int total_weight = 0;

    for (int i = 0; i <= static_cast<int>(BlockType::DIAMOND); i++)
    {
        BlockType block_type = static_cast<BlockType>(i);

        if (block_type == BlockType::BEDROCK)
            continue;

        std::optional<BlockData> optional_block_data = m_block_data_service->GetBlockData(block_type);
        if (!optional_block_data.has_value())
            continue;

        const BlockData& block_data = optional_block_data.value();

        if (depth < block_data.m_min_depth)
            continue;

        if (block_data.m_max_depth != -1 && depth > block_data.m_max_depth)
            continue;

        if (block_data.m_weight <= 0)
            continue;

        candidate_vector.push_back(make_pair(block_type, block_data.m_weight));
        total_weight += block_data.m_weight;
    }

    if (candidate_vector.empty())
        return BlockType::STONE;

    std::sort(candidate_vector.begin(), candidate_vector.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(1, 100);

    for (auto it = candidate_vector.rbegin(); it != candidate_vector.rend(); ++it)
    {
        const auto& [type, chance] = *it;

        if (dist(rng) <= chance)
            return type;
    }

    return BlockType::STONE;
}
