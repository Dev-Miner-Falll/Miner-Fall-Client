#include "Block.h"
#include "Utils/Pool/ObjectPoolManager.h"
#include "Game/FloatingItem/FloatingItem.h"
#include "Utils/Manager/SoundManager.h"

USING_NS_AX;

bool Block::init()
{
    if (!Node::init())
        return false;

    m_block_sprite = Sprite::createWithSpriteFrameName("Grass Dirt.png");
    m_block_sprite->setAnchorPoint(Vec2::ZERO);
    this->addChild(m_block_sprite);

    m_break_sprite = Sprite::createWithSpriteFrameName("Break1.png");
    m_break_sprite->setAnchorPoint(Vec2::ZERO);
    m_break_sprite->setContentSize(Vec2(32.f, 32.f));
    this->addChild(m_break_sprite);

    this->setContentSize(m_block_sprite->getContentSize());

    return true;
}

void Block::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();

    Vec2 body_size = Vec2(32.f, 32.f);
    m_physics_body = PhysicsBody::createBox(body_size, PhysicsMaterial(1.0f, 0.1f, 0.5f));
    m_physics_body->setDynamic(false);
    m_physics_body->setContactTestBitmask(0xFFFFFFFF);
    this->addComponent(m_physics_body);
}

void Block::Inject(const BlockData& block_data, int world_y_index)
{
    m_block_data = block_data;
    m_world_y_index = world_y_index;

    if (m_physics_body)
        m_physics_body->setPositionOffset(Vec2::ZERO);

    m_mining_time = 0.f;
    m_is_mining   = false;
    m_is_broken = false;

    UpdateMiningVisual();

    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_block_data.m_frame_name);
    m_block_sprite->setSpriteFrame(frame);

    this->setContentSize(m_block_sprite->getContentSize());
    this->setVisible(true);
}


void Block::StartMining(float delta)
{
    if (!m_block_data.m_is_breakable)
        return;

    m_mining_time += delta;

    UpdateMiningVisual();

    if (m_mining_time >= m_block_data.m_break_time)
        Break();

}

void Block::StopMining()
{
    m_is_mining = false;
}

void Block::Break()
{
    if (m_is_broken || !m_block_data.m_is_breakable)
        return;

    CreateBreakEffect();

    m_is_broken = true;

    if (m_physics_body)
        m_physics_body->setPositionOffset(Vec2(-10000, -10000));

    if (m_on_break)
        m_on_break(m_world_y_index, m_block_data.m_item_code);

    SoundManager::getInstance()->PlaySFX("Sounds/SFX/Break.mp3");
    
    setVisible(false);
}



void Block::UpdateMiningVisual()
{
    if (!m_block_data.m_is_breakable)
    {
        m_break_sprite->setVisible(false);
        return;
    }

    if (m_mining_time <= 0.f)
    {
        m_break_sprite->setVisible(false);
        return;
    }
    m_break_sprite->setVisible(true);

    float progress = m_mining_time / m_block_data.m_break_time;
    std::string frameName;

    if (progress < 0.25f)
        frameName = "Break1.png";
    else if (progress < 0.5f)
        frameName = "Break2.png";
    else if (progress < 0.75f)
        frameName = "Break3.png";
    else
        frameName = "Break4.png";

    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
    if (frame)
    {
        m_break_sprite->setSpriteFrame(frame);
    }
}

void Block::CreateBreakEffect()
{
    if (m_block_data.m_item_frame_name == "NONE")
        return;

    FloatingItem* floating_item = static_cast<FloatingItem*>(ObjectPoolManager::Get(ObjectType::FLOATING_ITEM));
    if (!floating_item)
        return;

    floating_item->setPosition(this->getPosition() + Vec2(16.f, 16.f));
    floating_item->SetFrame(m_block_data.m_item_frame_name);
    this->getParent()->addChild(floating_item, 100);

    floating_item->PlayFloatingAnimation();
}
