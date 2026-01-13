#include "axmol/axmol.h"
#include "ExtraBombStrategy.h"
#include "Utils/Pool/ObjectPoolManager.h"
#include "ExplosiveItem.h"
#include "Utils/Manager/SoundManager.h"

USING_NS_AX;

ExtraBombStrategy::ExtraBombStrategy()
    : ExplosiveItemStrategy(TILE_SIZE * 4, 5.f)
{}

void ExtraBombStrategy::Execute(const ItemActionContext& context)
{
    Vec2 bomb_position = context.m_pickaxe->getPosition();

    ExplosiveItem* explosive_item = static_cast<ExplosiveItem*>(ObjectPoolManager::Get(ObjectType::EXPLOSIVE_ITEM));
    explosive_item->Inject("Extra Bomb.png", context.m_pickaxe->getPosition(), m_radius, m_interval);
    explosive_item->Activate(context, 10.f, 1.5f);
    context.m_game_layer->addChild(explosive_item);

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Ignition.mp3");
}
