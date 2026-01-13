#pragma once

#include <string>
#include <memory>
#include "axmol/axmol.h"
#include "UI/IOpenableUI.h"
#include "Service/Item/IItemDataService.h"
#include "Service/Inventory/IInventoryService.h"
#include "Service/Shop/IShopDataService.h"

class ShopUI : public ax::Layer, public IOpenableUI
{
public:
    static ShopUI* create(std::shared_ptr<Service::IInventoryService> inventory_service,
                          std::shared_ptr<Service::IItemDataService> item_data_service,
                          std::shared_ptr<Service::IShopDataService> shop_data_service);
    virtual bool init(std::shared_ptr<Service::IInventoryService> inventory_service,
                      std::shared_ptr<Service::IItemDataService> item_data_service,
                      std::shared_ptr<Service::IShopDataService> shop_data_service);

public:
    void OpenUI() override;
    void CloseUI() override;

private:
    void ToggleUI(bool active);

    void CreateUI();
    void CreateShopPanel();
    void CreateTitleGroup();
    void CreateScrollView();
    void CreateShopSlots();

private:
    ax::Size m_canvas_size;
    ax::Size m_panel_size;

    ax::Node* m_shop_panel;
    ax::Node* m_scroll_view;

private:
    std::shared_ptr<Service::IInventoryService> m_inventory_service;
    std::shared_ptr<Service::IItemDataService> m_item_data_service;
    std::shared_ptr<Service::IShopDataService> m_shop_data_service;
};
