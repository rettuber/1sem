#pragma once
#include "Player.h"

class SaveManager
{
public:
    static bool Save(const Player& player);
    static bool Load(Player& player);
};