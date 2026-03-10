#include "Excavation.h"
#include "Utils.h"
#include <iostream>

using namespace std;

string DesertExcavation::GetName() const
{
    return "Пустыня";
}

int DesertExcavation::EnergyCost() const
{
    return 20;
}

vector<string> DesertExcavation::PossibleLoot() const
{
    return { "Аммонит", "Янтарь", "Редкий черепок" };
}

void DesertExcavation::Explore(Player& player)
{
    if (player.energy < EnergyCost())
    {
        cout << "Недостаточно энергии\n";
        return;
    }

    player.energy -= EnergyCost();

    auto loot = PossibleLoot();
    int count = RandomInt(1, 3);

    cout << "Вы нашли:\n";

    for (int i = 0; i < count; i++)
    {
        string item = loot[RandomInt(0, loot.size() - 1)];
        player.AddLoot(item, 1);

        cout << item << endl;
    }
}

string ForestExcavation::GetName() const
{
    return "Лес";
}

int ForestExcavation::EnergyCost() const
{
    return 18;
}

vector<string> ForestExcavation::PossibleLoot() const
{
    return { "Кость динозавра", "Окаменелый лист", "Аммонит" };
}

void ForestExcavation::Explore(Player& player)
{
    if (player.energy < EnergyCost())
    {
        cout << "Недостаточно энергии\n";
        return;
    }

    player.energy -= EnergyCost();

    auto loot = PossibleLoot();
    int count = RandomInt(1, 3);

    for (int i = 0; i < count; i++)
    {
        string item = loot[RandomInt(0, loot.size() - 1)];
        player.AddLoot(item, 1);

        cout << item << endl;
    }
}

string MountainExcavation::GetName() const
{
    return "Горы";
}

int MountainExcavation::EnergyCost() const
{
    return 25;
}

vector<string> MountainExcavation::PossibleLoot() const
{
    return { "Зуб хищника", "Фрагмент скелета" };
}

void MountainExcavation::Explore(Player& player)
{
    if (player.energy < EnergyCost())
    {
        cout << "Недостаточно энергии\n";
        return;
    }

    player.energy -= EnergyCost();

    auto loot = PossibleLoot();
    int count = RandomInt(2, 4);

    for (int i = 0; i < count; i++)
    {
        string item = loot[RandomInt(0, loot.size() - 1)];
        player.AddLoot(item, 1);

        cout << item << endl;
    }
}