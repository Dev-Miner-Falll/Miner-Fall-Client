#pragma once

#include "axmol/axmol.h"
#include "axmol/ui/UIScrollView.h"
#include "axmol/ui/UIScale9Sprite.h"
#include "axmol/ui/UIButton.h"
#include "UI/IOpenableUI.h"
#include "Service/Craft/IRecipeDataService.h"
#include "Service/Item/IItemDataService.h"
#include "Service/Inventory/IInventoryService.h"
#include "RecipeSlot.h"
#include "IngredientSlot.h"

class CraftUI : public ax::Layer, public IOpenableUI
{
public:
    static CraftUI* create(std::shared_ptr<Service::IRecipeDataService> recipe_data_service,
                           std::shared_ptr<Service::IItemDataService> item_data_service,
                           std::shared_ptr<Service::IInventoryService> inventory_service);

    virtual bool init(std::shared_ptr<Service::IRecipeDataService> recipe_data_service,
                      std::shared_ptr<Service::IItemDataService> item_data_service,
                      std::shared_ptr<Service::IInventoryService> inventory_service);

    void onEnter() override;
    void onExit() override;

public:
    void OpenUI() override;
    void CloseUI() override;

private:
    void CreateUI();
    void CreateCraftPanel();
    void CreateTitleGroup();
    void CreateRecipeLayer();
    void CreateRecipeSlots();
    void CreateCraftLayer();
    void CreateIngredientSlots(ItemCode item_code);
    void ReleaseIngredientSlots();
    void InitCrafter(ItemCode item_code);

    void ToggleUI(bool active);
    void ToggleLayer(bool is_craft_mode, bool can_jump = false);
    void OnClickedRecipe(ItemCode item_code);
    void OnClickedCraft();
    void UpdateUI(ax::EventCustom* event);
    bool CanCraft() const;

private:
    std::shared_ptr<Service::IRecipeDataService> m_recipe_data_service;
    std::shared_ptr<Service::IItemDataService> m_item_data_service;
    std::shared_ptr<Service::IInventoryService> m_inventory_service;

private:
    ax::Size m_canvas_size;
    ax::Size m_panel_size;

    ax::Layer* m_list_layer;
    ax::Layer* m_craft_layer;

    ax::Node* m_craft_panel;
    ax::ui::ScrollView* m_list_scroll_view;
    ax::ui::ScrollView* m_ingredient_scroll_view;
    ax::Sprite* m_portrait_sprite;
    ax::ui::Button* m_craft_button;
    ax::Label* m_product_name_label;
    ax::Label* m_product_count_label;

    ItemCode m_item_code;

    std::vector<IngredientSlot*> m_ingredient_vector;
    ax::EventListenerCustom* m_update_listener;
};
