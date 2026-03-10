#include "Player.h"

int Player::TotalToolLevel() const
{
    return pickaxe + shovel + brush;
}

void Player::AddLoot(const std::string& loot, int count)
{
    inventoryLoot[loot] += count;
}