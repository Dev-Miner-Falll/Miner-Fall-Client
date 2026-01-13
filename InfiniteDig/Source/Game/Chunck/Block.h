#pragma once

#include "axmol/axmol.h"
#include "Data/Block/BlockData.h"

class Block : public ax::Node
{
public:
    CREATE_FUNC(Block)
    bool init() override;
    void onEnterTransitionDidFinish() override;

    void Inject(const BlockData& block_data, int world_y_index);
    void setBreakCallback(std::function<void(int, ItemCode)> callback) { m_on_break = callback; }

public:
    void StartMining(float delta);
    void StopMining();

    bool IsBroken() const { return m_is_broken; }
    int GetWorldYIndex() const { return m_world_y_index; }

    void Break();

private:
    void UpdateMiningVisual();
    void CreateBreakEffect();

private:
    BlockData m_block_data;

    ax::Sprite* m_block_sprite;
    ax::Sprite* m_break_sprite;
    ax::PhysicsBody* m_physics_body;

    float m_mining_time;
    bool m_is_mining;
    bool m_is_broken;

    int m_world_y_index;
    std::function<void(int, ItemCode)> m_on_break;
};
