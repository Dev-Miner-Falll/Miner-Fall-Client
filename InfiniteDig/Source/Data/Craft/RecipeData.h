#pragma once

#include <vector>
#include "Data/Item/ItemCode.h"

struct IngredientData
{
    ItemCode m_ingredient_item_code;
    int m_ingredient_count;
};

struct RecipeData
{
    ItemCode m_product_code;
    int m_product_count;

    std::vector<IngredientData> m_ingredient_code_vector;
};
