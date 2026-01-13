#pragma once

#include "axmol/axmol.h"
#include "axmol/ui/UISlider.h"
#include "axmol/ui/UIScale9Sprite.h"
#include "UI/IOpenableUI.h"
#include "Service/Setting/ISettingService.h"

class SettingUI : public ax::Layer, public IOpenableUI
{
public:
    static SettingUI* create(std::shared_ptr<Service::ISettingService> setting_service);
    virtual bool init(std::shared_ptr<Service::ISettingService> setting_service);
    void onEnter() override;

public:
    void OpenUI() override;
    void CloseUI() override;

private:
    void InitUI();
    void ToggleUI(bool active);

    void CreateUI();
    void CreateBlockingLayer();
    void CreateSettingPanel();
    void CreateTitleGroup();
    void CreateToggleGroup();
    void CreateSliderGroup();
    void CreateExitButton();

    void OnClickedBGMPrintToggle(Object* sender);
    void OnClickedSFXPrintToggle(Object* sender);
    void OnBGMVolumeChanged(Object* sender, ax::ui::Slider::EventType type);
    void OnSFXVolumeChanged(Object* sender, ax::ui::Slider::EventType type);
    void OnClickedExit();

private:
    ax::Size m_canvas_size;
    ax::Size m_panel_size;

    std::shared_ptr<Service::ISettingService> m_setting_service;
    ax::EventListenerTouchOneByOne* m_block_listener;

private:
    ax::ui::Scale9Sprite* m_setting_panel;
    ax::Layer* m_block_layer;

    ax::MenuItemToggle* m_bgm_print_toggle;
    ax::MenuItemToggle* m_sfx_print_toggle;

    ax::ui::Slider* m_bgm_volume_slider;
    ax::ui::Slider* m_sfx_volume_slider;
};
