#include <string>
#include "LocalRecipeDataService.h"
#include "axmol/axmol.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

namespace Service
{
    LocalRecipeDataService::LocalRecipeDataService()
    {
        Load();
    }



    const std::vector<RecipeData>& LocalRecipeDataService::GetRecipeDatas() const
    {
        return m_recipe_vector;
    }

    std::optional<RecipeData> LocalRecipeDataService::GetRecipeData(ItemCode item_code) const
    {
        if (m_recipe_map.find(item_code) == m_recipe_map.end())
            return std::nullopt;

        return m_recipe_map.at(item_code);
    }



    void LocalRecipeDataService::Load()
    {
        using namespace std;
        using namespace rapidjson;

        string file_path = ax::FileUtils::getInstance()->getWritablePath() + "RecipeData.json";
        string json_str  = ax::FileUtils::getInstance()->getStringFromFile(file_path);

        AXASSERT(!json_str.empty(), "RecipeData.json 로드에 실패했습니다.");

        Document doc;
        doc.Parse(json_str.c_str());

        AXASSERT(doc.IsObject(), "RecipeData.json의 형식이 잘못되었습니다.");
        AXASSERT(doc.HasMember("recipes"), "RecipeData.json의 형식이 잘못되었습니다.");

        const Value& recipes = doc["recipes"];
        AXASSERT(recipes.IsArray(), "recipes는 배열이 아닙니다.");

        m_recipe_vector.clear();
        for (const auto& recipe : recipes.GetArray())
        {
            RecipeData recipe_data;

            recipe_data.m_product_code = static_cast<ItemCode>(recipe["product_code"].GetInt());
            recipe_data.m_product_count = recipe["product_count"].GetInt();

            const Value& ingredients = recipe["ingredients"];
            AXASSERT(ingredients.IsArray(), "ingredients는 배열이 아닙니다.");

            for (const auto& ingredient : ingredients.GetArray())
            {
                IngredientData ingredient_data;

                ingredient_data.m_ingredient_item_code = static_cast<ItemCode>(ingredient["ingredient_code"].GetInt());
                ingredient_data.m_ingredient_count     = ingredient["ingredient_count"].GetInt();

                recipe_data.m_ingredient_code_vector.push_back(ingredient_data);
            }

            m_recipe_map[recipe_data.m_product_code] = recipe_data;
            m_recipe_vector.push_back(recipe_data);
        }
    }
}
