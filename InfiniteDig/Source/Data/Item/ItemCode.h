#pragma once

enum class ItemCode : int
{
    NONE = 0,

    COAL_STUFF     = 1,
    IRON_STUFF     = 2,
    GOLD_STUFF     = 3,
    EMERALD_STUFF  = 4,
    AMETHYST_STUFF = 5,
    DIAMOND_STUFF  = 6,

    COAL_INGOT = 101,
    IRON_INGOT = 102,
    GOLD_INGOT = 103,

    EMERALD_REFINE  = 201,
    AMETHYST_REFINE = 202,
    DIAMOND_REFINE  = 203,

    SMALL_DYNAMITE = 301,
    MEDIUM_DYNAMITE = 302,
    MEDIUM_BOMB = 303,
    EXTRA_BOMB = 304,
};

constexpr ItemCode ALL_ITEM_CODES[]
{
    ItemCode::COAL_STUFF,
    ItemCode::IRON_STUFF,
    ItemCode::GOLD_STUFF,
    ItemCode::EMERALD_STUFF,
    ItemCode::AMETHYST_STUFF,
    ItemCode::DIAMOND_STUFF,

    ItemCode::COAL_INGOT,
    ItemCode::IRON_INGOT,
    ItemCode::GOLD_INGOT,

    ItemCode::EMERALD_REFINE,
    ItemCode::AMETHYST_REFINE,
    ItemCode::DIAMOND_REFINE,

    ItemCode::SMALL_DYNAMITE,
    ItemCode::MEDIUM_DYNAMITE,
    ItemCode::MEDIUM_BOMB,
    ItemCode::EXTRA_BOMB,
};
