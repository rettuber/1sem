#include "Game.h"
#include "Utils.h"
#include "Excavation.h"
#include "SaveManager.h"
#include <iostream>

using namespace std;

void Game::Start()
{
    DesertExcavation desert;
    ForestExcavation forest;
    MountainExcavation mountain;

    while (true)
    {
        cout << "\n1 Экспедиция\n";
        cout << "2 Сохранить\n";
        cout << "3 Загрузить\n";
        cout << "4 Выход\n";

        int choice = ReadInt(1, 4);

        if (choice == 1)
        {
            cout << "1 Пустыня\n2 Лес\n3 Горы\n";

            int c = ReadInt(1, 3);

            if (c == 1) desert.Explore(player);
            if (c == 2) forest.Explore(player);
            if (c == 3) mountain.Explore(player);
        }

        if (choice == 2)
            SaveManager::Save(player);

        if (choice == 3)
            SaveManager::Load(player);

        if (choice == 4)
            break;
    }
}