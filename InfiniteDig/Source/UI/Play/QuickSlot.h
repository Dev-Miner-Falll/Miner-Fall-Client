#pragma once

#include <functional>
#include "axmol/axmol.h"
#include "Data/Item/ItemData.h"

class QuickSlot : public ax::Sprite
{
public:
    CREATE_FUNC(QuickSlot)
    bool init() override;
    void update(float delta) override;
    void onEnter() override;
    void onExit() override;
    void Inject(ItemData item_data, int slot_index);

public:
    void StartCooldown();
    bool IsCoolingDown() const;

    void SetOnClickEvent(std::function<void(int)> callback);

    ItemCode GetItemCode() const { return m_item_data.m_item_code; }

private:
    void CreateUI();
    void CreateFrame();
    void CreateMask();
    void CreateCountLabel();

    void UpdateCoolingMask();
    void UpdateUI(ax::EventCustom* event);

private:
    ax::Size m_content_size;
    int m_slot_index;

    ItemData m_item_data;
    float m_left_cooltime;

    bool m_is_cooling;
    bool m_is_active;

    ax::Sprite* m_icon_sprite;
    ax::ProgressTimer* m_cool_mask;
    ax::Label* m_count_label;

    std::function<void(int)> m_on_click;
    ax::EventListenerTouchOneByOne* m_touch_listener;
    ax::EventListenerCustom* m_update_listener;
};
