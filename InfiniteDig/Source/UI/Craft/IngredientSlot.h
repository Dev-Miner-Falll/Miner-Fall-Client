#include "axmol/axmol.h"
#include "axmol/ui/UIButton.h"
#include "axmol/ui/UIScale9Sprite.h"
#include "Data/Item/ItemData.h"
#include "Data/Craft/RecipeData.h"
#include "Utils/Pool/IPoolable.h"
#include "Service/Inventory/IInventoryService.h"

class IngredientSlot : public ax::Sprite, public IPoolable
{
public:
    CREATE_FUNC(IngredientSlot)
    bool init() override;
    void onEnter() override;
    void onExit() override;

    void InitUI(ItemData item_data,
                IngredientData ingredient_data,
                std::shared_ptr<Service::IInventoryService> inventory_service);

public:
    void OnEnable() override;
    void OnDisable() override;
    ObjectType GetObjectType() const override;

private:
    void CreateUI();
    void CreateSlotPanel();
    void CreatePortrait();
    void CreateDescriptionLabel();

    void UpdateUI(ax::EventCustom* event);

private:
    ax::Size m_slot_size;

    ax::ui::Scale9Sprite* m_slot_panel;
    ax::Sprite* m_portrait_sprite;
    ax::Label* m_name_label;
    ax::Label* m_count_label;

    ItemData m_item_data;
    IngredientData m_ingredient_data;
    ax::EventListenerCustom* m_update_listener;

    std::shared_ptr<Service::IInventoryService> m_inventory_service;
};
