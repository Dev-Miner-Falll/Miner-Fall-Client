#include "ExplosiveItem.h"
#include "Utils/Pool/ObjectPoolManager.h"
#include "Utils/Manager/SoundManager.h"

USING_NS_AX;

bool ExplosiveItem::init()
{
    if (!Sprite::init())
        return false;

    return true;
}

void ExplosiveItem::onEnter()
{
    Sprite::onEnter();

    m_body = PhysicsBody::createCircle(16.f, PhysicsMaterial(0.8f, 0.8f, 0.1f));
    m_body->setDynamic(true);
    m_body->setGravityEnable(true);
    m_body->setRotationEnable(true);
    m_body->setContactTestBitmask(0xFFFFFFFF);
    this->addComponent(m_body);
}



void ExplosiveItem::OnEnable()
{
    this->setVisible(true);
    this->setColor(Color32::WHITE);
}

void ExplosiveItem::OnDisable()
{
    this->stopAllActions();
    this->setVisible(false);

    if (m_body)
    {
        this->removeComponent(m_body);
        m_body = nullptr;
    }
}

ObjectType ExplosiveItem::GetObjectType() const
{
    return ObjectType::EXPLOSIVE_ITEM;
}



void ExplosiveItem::Inject(std::string frame_name, Vec2 position, float radius, float delay)
{
    SpriteFrame* target_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frame_name);
    this->setSpriteFrame(target_frame);
    this->setPosition(position);

    m_radius = radius;
    m_delay  = delay;
}

void ExplosiveItem::Activate(const ItemActionContext& action_context, float shaking_strength, float shaking_duration)
{
    CreateBlinkAction();
    CreateExplosion(action_context, shaking_strength, shaking_duration);
}



void ExplosiveItem::CreateBlinkAction()
{
    const float COLOR_INTERVAL   = 0.5f;

    TintTo* to_white_action      = TintTo::create(COLOR_INTERVAL, Color32::WHITE);
    TintTo* to_black_action      = TintTo::create(COLOR_INTERVAL, Color32::BLACK);
    Sequence* seqeunce_action    = Sequence::create(to_white_action, to_black_action, nullptr);
    RepeatForever* repeat_action = RepeatForever::create(seqeunce_action);

    this->runAction(repeat_action);
}

void ExplosiveItem::CreateExplosion(const ItemActionContext& action_context,
                                    float shaking_strength,
                                    float shaking_duration)
{
    DelayTime* delay_action = DelayTime::create(m_delay);

    CallFunc* func_action = CallFunc::create([this, action_context, shaking_strength, shaking_duration]()
    {
        std::vector<Block*> block_vector = action_context.m_chunck_manager->GetBlocksCollidedWithPickaxe(this->getPosition(),
                                                                                                         m_radius);
        for (Block* block : block_vector)
            block->Break();

        action_context.m_layer_shaker->Shake(shaking_strength, shaking_duration);

        
        SoundManager::getInstance()->PlaySFX("Sounds/SFX/Small Dynamite.mp3");
    });

    CallFunc* release_action = CallFunc::create([this]()
    {
        ObjectPoolManager::Release(this);
    });

    Sequence* sequence_action = Sequence::create(delay_action, func_action, release_action, nullptr);

    this->runAction(sequence_action);
}
