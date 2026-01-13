#include "FloatingItem.h"
#include "Utils/Pool/ObjectPoolManager.h"

USING_NS_AX;

bool FloatingItem::init()
{
    if (!Sprite::init())
        return false;

    return true;
}



void FloatingItem::OnEnable()
{
    this->setVisible(true);
    this->setOpacity(255);
}

void FloatingItem::OnDisable()
{
    this->stopAllActions();
    this->setVisible(false);
}

ObjectType FloatingItem::GetObjectType() const
{
    return ObjectType::FLOATING_ITEM;
}



void FloatingItem::SetFrame(std::string frame_name)
{
    SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frame_name);
    this->setSpriteFrame(frame);
}



void FloatingItem::PlayFloatingAnimation()
{
    MoveBy* move_action       = MoveBy::create(0.8f, Vec2(0.f, 50.f));
    FadeOut* fade_action      = FadeOut::create(0.8f);
    Spawn* spawn_action       = Spawn::create(move_action, fade_action, nullptr);
    CallFunc* release_action  = CallFunc::create([this]() { ObjectPoolManager::Release(this); });
    Sequence* sequence_action = Sequence::create(spawn_action, release_action, nullptr);
    this->runAction(sequence_action);
}
