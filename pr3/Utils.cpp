#include "Utils.h"
#include <iostream>
#include <random>
#include <ctime>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void SetupConsoleRussian()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "");
}

int ReadInt(int minValue, int maxValue)
{
    int value;

    while (true)
    {
        cout << "> ";

        if (cin >> value && value >= minValue && value <= maxValue)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Некорректный ввод\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int RandomInt(int from, int to)
{
    static mt19937 rng((unsigned)time(nullptr));
    uniform_int_distribution<int> dist(from, to);
    return dist(rng);
}