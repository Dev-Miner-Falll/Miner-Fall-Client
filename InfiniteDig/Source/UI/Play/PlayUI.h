#pragma once

#include <vector>
#include "axmol/axmol.h"
#include "UI/IOpenableUI.h"
#include "Service/Item/IItemDataService.h"
#include "Service/Inventory/IInventoryService.h"
#include "QuickSlot.h"
#include "Game/Chunck/ChunckManager.h"
#include "Game/Pickaxe.h"
#include "Utils/LayerShaker.h"

class PlayUI : public ax::Layer, public IOpenableUI
{
public:
    static PlayUI* create(std::shared_ptr<Service::IInventoryService> inventory_service,
                          std::shared_ptr<Service::IItemDataService> item_data_service,
                          Pickaxe* pickaxe,
                          ChunckManager* chunck_manager,
                          LayerShaker* layer_shaker);
    virtual bool init(std::shared_ptr<Service::IInventoryService> inventory_service,
                      std::shared_ptr<Service::IItemDataService> item_data_service,
                      Pickaxe* pickaxe,
                      ChunckManager* chunck_manager,
                      LayerShaker* layer_shaker);

public:
    void OpenUI() override;
    void CloseUI() override;

    void UpdateUI(int depth);

private:
    void ToggleUI(bool active);

    void CreateUI();
    void CreateQuickSlots();
    void CreateDepthLabel();

    void OnClickedSlot(int slot_index);
    void PlaySlotAction(int slot_index);

private:
    std::shared_ptr<Service::IItemDataService> m_item_data_service;
    std::shared_ptr<Service::IInventoryService> m_inventory_service;

    ax::Size m_canvas_size;

    std::vector<QuickSlot*> m_slot_vector;
    ax::Label* m_depth_label;

    Pickaxe* m_pickaxe;
    ChunckManager* m_chunck_manager;
    LayerShaker* m_layer_shaker;
};
