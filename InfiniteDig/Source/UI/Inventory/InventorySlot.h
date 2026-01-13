#pragma once

#include <string>
#include "axmol/axmol.h"
#include "Data/Item/ItemData.h"

class InventorySlot : public ax::Sprite
{
public:
    CREATE_FUNC(InventorySlot)
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void InitUI(ItemData item_data);
private:
    void CreateUI();
    void UpdateUI(ax::EventCustom* event);

private:
    ax::Size m_canvas_size;
    ax::Size m_panel_size;

    ax::Sprite* m_item_portrait_sprite;
    ax::Label* m_item_count_label;

    ItemCode m_item_code;
    ax::EventListenerCustom* m_update_listener;
};
