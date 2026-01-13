#include "axmol/axmol.h"
#include "MediumDynamiteStrategy.h"
#include "Utils/Pool/ObjectPoolManager.h"
#include "ExplosiveItem.h"
#include "Utils/Manager/SoundManager.h"

USING_NS_AX;

MediumDynamiteStrategy::MediumDynamiteStrategy()
    : ExplosiveItemStrategy(TILE_SIZE * 2, 3.f)
{}

void MediumDynamiteStrategy::Execute(const ItemActionContext& context)
{
    Vec2 bomb_position = context.m_pickaxe->getPosition();

    ExplosiveItem* explosive_item = static_cast<ExplosiveItem*>(ObjectPoolManager::Get(ObjectType::EXPLOSIVE_ITEM));
    explosive_item->Inject("Medium Dynamite.png", context.m_pickaxe->getPosition(), m_radius, m_interval);
    explosive_item->Activate(context, 5.f, 0.75f);
    context.m_game_layer->addChild(explosive_item);

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Ignition.mp3");
}
