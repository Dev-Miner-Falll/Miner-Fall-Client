#pragma once

#include <string>
#include "axmol/axmol.h"
#include "axmol/ui/UIButton.h"
#include "Data/Shop/ShopData.h"
#include "Data/Item/ItemData.h"
#include "Service/Inventory/IInventoryService.h"

class ShopSlot : public ax::Sprite
{
public:
    CREATE_FUNC(ShopSlot)
    bool init() override;
    void onEnter() override;
    void onExit() override;

public:
    void InitUI(std::shared_ptr<Service::IInventoryService> inventory_service,
                ShopData shop_data,
                ItemData item_data);

private:
    void UpdateUI(ax::EventCustom* event);

    void CreateUI();
    void CreateSlotPanel();
    void CreatePortraitGroup();
    void CreateDescriptionGroup();
    void CreatePurchaseButton();

    void OnClickedPurchase();
    
private:
    ax::Size m_panel_size;
  
    ax::Node* m_slot_panel;
    ax::Sprite* m_portrait_sprite;
    ax::Label* m_name_label;
    ax::Label* m_description_label;
    ax::ui::Button* m_purchase_button;

private:
    std::shared_ptr<Service::IInventoryService> m_inventory_service;

    ItemCode m_item_code;
    int m_item_cost;

    ax::EventListenerCustom* m_update_listener;
};
