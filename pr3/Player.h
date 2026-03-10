#pragma once
#include <map>
#include <string>

struct Player
{
    int money = 100;
    int energy = 100;
    int day = 1;

    int pickaxe = 1;
    int shovel = 1;
    int brush = 1;
    int food = 3;

    std::map<std::string, int> inventoryLoot;
    std::map<std::string, int> museumCollection;

    int TotalToolLevel() const;
    void AddLoot(const std::string& loot, int count);
};