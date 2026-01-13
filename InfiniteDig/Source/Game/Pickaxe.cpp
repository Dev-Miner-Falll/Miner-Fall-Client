#include "Pickaxe.h"

USING_NS_AX;

Pickaxe* Pickaxe::create(ChunckManager* chunck_manager)
{
    Pickaxe* pickaxe = new (std::nothrow) Pickaxe();
    if (pickaxe && pickaxe->init(chunck_manager))
    {
        pickaxe->autorelease();
        return pickaxe;
    }

    AX_SAFE_DELETE(pickaxe);
    return nullptr;
}

bool Pickaxe::init(ChunckManager* chunck_manager)
{
    if (!Node::init())
        return false;

    m_chunck_manager = chunck_manager;

    auto sprite = Sprite::create("Game/Object/pickaxes.png", Rect(0, 0, 32, 32));
    sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->addChild(sprite);

    m_radius = 16.f;

    return true;
}

void Pickaxe::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();

    PhysicsBody* body = PhysicsBody::createCircle(m_radius, PhysicsMaterial(0.8f, 0.8f, 0.1f));
    body->setDynamic(true);
    body->setGravityEnable(true);
    body->setRotationEnable(true);
    body->setContactTestBitmask(0xFFFFFFFF);
    this->addComponent(body);

    scheduleUpdate();
}

void Pickaxe::update(float dt)
{
    if (m_chunck_manager)
        m_chunck_manager->UpdatePickaxe(getPosition(), m_radius, dt);
}
