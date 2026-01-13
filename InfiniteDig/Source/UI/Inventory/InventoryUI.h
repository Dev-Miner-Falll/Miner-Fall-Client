#pragma once

#include <string>
#include "axmol/axmol.h"
#include "UI/IOpenableUI.h"
#include "Service/Inventory/IInventoryService.h"
#include "Service/Item/IItemDataService.h"

class InventoryUI : public ax::Layer, public IOpenableUI
{
public:
    static InventoryUI* create(std::shared_ptr<Service::IInventoryService> inventory_service,
                               std::shared_ptr<Service::IItemDataService> item_data_service);
    virtual bool init(std::shared_ptr<Service::IInventoryService> inventory_service,
                      std::shared_ptr<Service::IItemDataService> item_data_service);

public:
    void OpenUI() override;
    void CloseUI() override;

private:
    void ToggleUI(bool active);

    void CreateUI();
    void CreateInventoryPanel();
    void CreateTitleGroup();
    void CreateToggleGroup();
    void CreateStuffInventory();
    void CreateRefineInventory();
    void CreateToolInventory();
    void CreateInventorySlots();

    void OnClickedStuff();
    void OnClickedRefine();
    void OnClickedTool();
    void SetSelectToggle(ax::MenuItemToggle* target_toggle);

private:
    ax::Size m_canvas_size;
    ax::Size m_panel_size;

    ax::Node* m_inventory_panel;

    ax::MenuItemToggle* m_stuff_toggle;
    ax::MenuItemToggle* m_refine_toggle;
    ax::MenuItemToggle* m_tool_toggle;

    ax::Node* m_stuff_inventory_layer;
    ax::Node* m_refine_inventory_layer;
    ax::Node* m_tool_inventory_layer;

private:
    std::shared_ptr<Service::IInventoryService> m_inventory_service;
    std::shared_ptr<Service::IItemDataService> m_item_data_service;
};
