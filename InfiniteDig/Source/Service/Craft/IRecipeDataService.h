#pragma once

#include <vector>
#include <optional>
#include "Service/ILoadable.h"
#include "Data/Craft/RecipeData.h"

namespace Service
{
    class IRecipeDataService : public ILoadable
    {
    public:
        virtual ~IRecipeDataService()                                             = default;
        virtual const std::vector<RecipeData>& GetRecipeDatas() const             = 0;
        virtual std::optional<RecipeData> GetRecipeData(ItemCode item_code) const = 0;
    };
}
