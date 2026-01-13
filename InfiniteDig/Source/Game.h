#pragma once

#include <memory>
#include "axmol/axmol.h"
#include "Service/ServiceLocator.h"

class Game : public ax::Scene
{
public:
    enum LayerOrder
    {
        BACKGROUND_LAYER = 0,
        SYSTEM_LAYER = 1,
        GAMEOBJECT_LAYER = 2,
        UI_LAYER = 3,
        SETTING_LAYER = 4
    };

public:
    CREATE_FUNC(Game)
    bool init() override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void update(float delta) override;

public:
    ax::Layer* GetLayer(LayerOrder layer) const;

private:
    void InitPhysics();
    void RegisterService();

    void CreateLayers();
    void CreateInGame();
    void CreateUIs();

private:
    std::shared_ptr<Service::IItemDataService> m_item_data_service;
    std::shared_ptr<Service::IInventoryService> m_inventory_service;
    std::shared_ptr<Service::IBlockDataService> m_block_data_service;
    std::shared_ptr<Service::IBackgroundDataService> m_background_data_service;
    std::shared_ptr<Service::IShopDataService> m_shop_data_service;
    std::shared_ptr<Service::IRecipeDataService> m_recipe_data_service;
    std::shared_ptr<Service::ISettingService> m_setting_service;

private:
    ax::Size m_canvas_size;

    ax::Layer* m_background_layer;
    ax::Layer* m_system_layer;
    ax::Layer* m_gameobject_layer;
    ax::Layer* m_ui_layer;
    ax::Layer* m_setting_layer;

    ax::Node* m_toggle_ui;
    ax::Node* m_play_ui;
    ax::Node* m_inventory_ui;
    ax::Node* m_shop_ui;
    ax::Node* m_craft_ui;
    ax::Node* m_setting_ui;

    ax::Node* m_pickaxe;
    ax::Node* m_chunck_manager;
    ax::Node* m_layer_shaker;

private:
    float m_start_system_y = 0.f;
    int m_current_depth    = 0;
};
