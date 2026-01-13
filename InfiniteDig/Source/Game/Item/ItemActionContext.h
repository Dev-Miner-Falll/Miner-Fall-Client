#pragma once

#include "axmol/axmol.h"
#include "Game/Pickaxe.h"
#include "Game/Chunck/ChunckManager.h"
#include "Utils/LayerShaker.h"

struct ItemActionContext
{
    Pickaxe* m_pickaxe;
    ChunckManager* m_chunck_manager;
    LayerShaker* m_layer_shaker;
    ax::Node* m_game_layer;
};
