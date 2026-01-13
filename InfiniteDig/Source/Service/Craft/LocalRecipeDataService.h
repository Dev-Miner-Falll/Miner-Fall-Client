#pragma once

#include <unordered_map>
#include "IRecipeDataService.h"

namespace Service
{
    class LocalRecipeDataService : public IRecipeDataService
    {
    public:
        const std::vector<RecipeData>& GetRecipeDatas() const override;
        std::optional<RecipeData> GetRecipeData(ItemCode item_code) const override;

    public:
        void Load();

    public:
        LocalRecipeDataService();

        LocalRecipeDataService(const LocalRecipeDataService&)            = delete;
        LocalRecipeDataService& operator=(const LocalRecipeDataService&) = delete;

    private:
        std::vector<RecipeData> m_recipe_vector;
        std::unordered_map<ItemCode, RecipeData> m_recipe_map;
    };
}
