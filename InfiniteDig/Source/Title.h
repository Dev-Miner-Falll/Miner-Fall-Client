#pragma once

#include "axmol/axmol.h"

class Title : public ax::Scene
{
public:
    CREATE_FUNC(Title);
    bool init() override;
    void onEnter() override;
    void onExit() override;

private:
    void RegisterSpriteFrame();
    void CreateLayer();

    void CreateUI();
    void CreateBackground();
    void CreateTitleLogo();
    void CreateTouchLabel();

private:
    ax::EventListenerTouchOneByOne* m_touch_listener;

    ax::Size m_canvas_size;

    ax::Layer* m_background_layer;
    ax::Layer* m_ui_layer;
};
