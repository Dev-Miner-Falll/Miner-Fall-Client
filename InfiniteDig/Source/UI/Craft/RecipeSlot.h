#pragma once

#include "axmol/axmol.h"
#include "axmol/ui/UIButton.h"
#include "axmol/ui/UIScale9Sprite.h"
#include "Data/Item/ItemData.h"

class RecipeSlot : public ax::Sprite
{
public:
    CREATE_FUNC(RecipeSlot)
    bool init() override;

    void InitUI(ItemData item_data, std::function<void(ItemCode)> click_callback);

private:
    void CreateUI();
    void CreateSlotPanel();
    void CreatePortrait();
    void CreateNameLabel();
    void CreateCraftButton();

    void OnClickedButton();

private:
    ax::Size m_slot_size;

    ax::ui::Scale9Sprite* m_slot_panel;
    ax::Sprite* m_portrait_sprite;
    ax::Label* m_name_label;
    ax::ui::Button* m_craft_button;

    ItemData m_item_data;
    std::function<void(ItemCode)> m_click_callback;
};
