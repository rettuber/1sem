#include "SaveManager.h"
#include <fstream>

using namespace std;

bool SaveManager::Save(const Player& player)
{
    ofstream file("save.txt");

    if (!file)
        return false;

    file << player.money << endl;
    file << player.energy << endl;
    file << player.day << endl;

    return true;
}

bool SaveManager::Load(Player& player)
{
    ifstream file("save.txt");

    if (!file)
        return false;

    file >> player.money;
    file >> player.energy;
    file >> player.day;

    return true;
}