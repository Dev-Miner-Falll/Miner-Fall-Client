#pragma once

#include "axmol/axmol.h"
#include "Game/Chunck/ChunckManager.h"

class Pickaxe : public ax::Node
{
public:
    static Pickaxe* create(ChunckManager* chunck_manager);
    virtual bool init(ChunckManager* chunck_manager);
    void onEnterTransitionDidFinish() override;
    void update(float dt) override;

private:
    float m_radius;

    ChunckManager* m_chunck_manager;
};
