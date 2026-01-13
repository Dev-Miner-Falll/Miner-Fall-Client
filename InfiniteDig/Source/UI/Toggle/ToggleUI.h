#pragma once

#include "axmol/axmol.h"
#include "UI/IOpenableUI.h"

class ToggleUI : public ax::Layer
{
public:
    static ToggleUI* create(IOpenableUI* play_ui,
                            IOpenableUI* inventory_ui,
                            IOpenableUI* shop_ui,
                            IOpenableUI* craft_ui,
                            IOpenableUI* setting_ui,
                            ax::Layer* setting_layer);
    virtual bool init(IOpenableUI* play_ui,
                      IOpenableUI* inventory_ui,
                      IOpenableUI* shop_ui,
                      IOpenableUI* craft_ui,
                      IOpenableUI* setting_ui,
                      ax::Layer* setting_layer);

private:
    void CreateUI();
    void CreateToggleMenu();
    void CreateDigToggle();
    void CreateShopToggle();
    void CreateCraftToggle();
    void CreateInventoryToggle();
    void CreateSettingButton();

    void OnClickedDigToggle();
    void OnClickedShopToggle();
    void OnClickedCraftToggle();
    void OnClickedInventoryToggle();
    void OnClickedSettingButton();
    void SetSelectedToggle(ax::MenuItemToggle* target_toggle);

private:
    ax::Size m_canvas_size;
    ax::Layer* m_setting_layer;

    ax::MenuItemToggle* m_dig_toggle;
    ax::MenuItemToggle* m_shop_toggle;
    ax::MenuItemToggle* m_craft_toggle;
    ax::MenuItemToggle* m_inventory_toggle;

    ax::Node* m_setting_button;

    IOpenableUI* m_play_ui;
    IOpenableUI* m_inventory_ui;
    IOpenableUI* m_shop_ui;
    IOpenableUI* m_craft_ui;
    IOpenableUI* m_setting_ui;
};
