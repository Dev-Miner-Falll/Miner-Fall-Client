#pragma once

#include "axmol/axmol.h"
#include "Service/Background/IBackgroundDataService.h"

class BackgroundManager : public ax::Node
{
public:
    static BackgroundManager* create(std::shared_ptr<Service::IBackgroundDataService> background_service);
    virtual bool init(std::shared_ptr<Service::IBackgroundDataService> background_service);

    void ReplaceBackground(int depth);
    void ReplaceBackground(std::string frame_name, int tile_pixel, float duration = 0.5f);

private:
    ax::Node* CreateBackgroundContainer(std::string frame_name, int tile_pixel);

private:
    std::shared_ptr<Service::IBackgroundDataService> m_background_service;

    ax::Size m_canvas_size;

    ax::Node* m_old_layer;
    ax::Node* m_current_layer;
    int m_current_index;

    int m_width_tile_count;
    int m_height_tile_count;
};
