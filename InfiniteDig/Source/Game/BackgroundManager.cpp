#include "BackgroundManager.h"

using namespace std;
USING_NS_AX;

BackgroundManager* BackgroundManager::create(std::shared_ptr<Service::IBackgroundDataService> background_service)
{
    BackgroundManager* background_manager = new (std::nothrow) BackgroundManager();
    if (background_manager && background_manager->init(background_service))
    {
        background_manager->autorelease();
        return background_manager;
    }

    AX_SAFE_DELETE(background_manager);
    return nullptr;
}

bool BackgroundManager::init(std::shared_ptr<Service::IBackgroundDataService> background_service)
{
    if (!Node::init())
        return false;

    m_background_service = background_service;

    m_canvas_size = _director->getCanvasSize();
    m_current_index = -1;

    return true;
}

void BackgroundManager::ReplaceBackground(int depth)
{
    for (const auto& data : m_background_service->GetBackgroundDatas())
    {
        if (depth >= data.m_min_depth)
        {
            if (m_current_index == data.m_index)
                return;

            ReplaceBackground(data.m_frame_name, data.m_tile_pixel);
            m_current_index = data.m_index;

            return;
        }
    }
}

void BackgroundManager::ReplaceBackground(string frame_name, int tile_pixel, float duration)
{
    Node* new_layer = CreateBackgroundContainer(frame_name, tile_pixel);
    new_layer->setCascadeOpacityEnabled(true);
    new_layer->setOpacity(0);
    this->addChild(new_layer, 1);

    if (m_current_layer)
    {
        m_old_layer = m_current_layer;
        m_old_layer->setLocalZOrder(0);
        m_old_layer->stopAllActions();
    }

    auto fade_in   = FadeIn::create(duration);
    auto callback = CallFunc::create([this]()
    {
        if (m_old_layer)
        {
            m_old_layer->removeFromParent();
            m_old_layer = nullptr;
        }
    });

    new_layer->runAction(Sequence::create(fade_in, callback, nullptr));
    m_current_layer = new_layer;
}

ax::Node* BackgroundManager::CreateBackgroundContainer(string frame_name, int tile_pixel)
{
    Node* container = Node::create();
    container->setCascadeOpacityEnabled(true);

    m_width_tile_count  = static_cast<int>(m_canvas_size.x / tile_pixel) + 1;
    m_height_tile_count = static_cast<int>(m_canvas_size.y / tile_pixel) + 1;

    for (int x = 0; x < m_width_tile_count; x++)
    {
        for (int y = 0; y < m_height_tile_count; y++)
        {
            Vec2 background_tile_position  = Vec2(x * tile_pixel, (y - 1) * tile_pixel);
            Sprite* background_tile_sprite = Sprite::createWithSpriteFrameName(frame_name);
            background_tile_sprite->getTexture()->setAliasTexParameters();
            background_tile_sprite->setAnchorPoint(Point::ZERO);
            background_tile_sprite->setPosition(background_tile_position);
            container->addChild(background_tile_sprite);
        }
    }

    auto move_action     = MoveBy::create(2.0f, Point(0, tile_pixel));
    auto place_action    = Place::create(Point::ZERO);
    auto sequence_action = Sequence::create(move_action, place_action, NULL);
    auto repeat_action   = RepeatForever::create(sequence_action);
    container->runAction(repeat_action);

    return container;
}
