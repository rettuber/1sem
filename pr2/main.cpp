#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <locale>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

void SetupConsole()
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
#endif
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    std::wcin.imbue(std::locale());
}

class Random
{
private:
    std::mt19937 gen;

public:
    Random() : gen(static_cast<unsigned int>(std::time(nullptr))) {}

    int NextInt(int left, int right)
    {
        std::uniform_int_distribution<int> dist(left, right);
        return dist(gen);
    }
};

Random rng;

class Client
{
private:
    int id;
    int itemsCount;
    int serviceTime;

public:
    Client() : id(0), itemsCount(0), serviceTime(0) {}

    Client(int id, int itemsCount, int serviceTime)
        : id(id), itemsCount(itemsCount), serviceTime(serviceTime) {
    }

    int GetId() const { return id; }
    int GetItemsCount() const { return itemsCount; }
    int GetServiceTime() const { return serviceTime; }

    std::wstring ToString() const
    {
        return L"Клиент #" + std::to_wstring(id) +
            L" [товаров: " + std::to_wstring(itemsCount) +
            L", время: " + std::to_wstring(serviceTime) + L" сек.]";
    }
};

template <typename T>
struct Node
{
    T data;
    Node<T>* prev;
    Node<T>* next;

    Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};

template <typename T>
class DoublyLinkedList
{
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList()
    {
        Clear();
    }

    void PushBack(const T& value)
    {
        Node<T>* node = new Node<T>(value);

        if (tail == nullptr)
        {
            head = tail = node;
        }
        else
        {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }

        ++size;
    }

    void PushFront(const T& value)
    {
        Node<T>* node = new Node<T>(value);

        if (head == nullptr)
        {
            head = tail = node;
        }
        else
        {
            node->next = head;
            head->prev = node;
            head = node;
        }

        ++size;
    }

    bool PopFront(T& outValue)
    {
        if (head == nullptr)
            return false;

        Node<T>* temp = head;
        outValue = head->data;

        head = head->next;
        if (head != nullptr)
            head->prev = nullptr;
        else
            tail = nullptr;

        delete temp;
        --size;
        return true;
    }

    bool PopBack(T& outValue)
    {
        if (tail == nullptr)
            return false;

        Node<T>* temp = tail;
        outValue = tail->data;

        tail = tail->prev;
        if (tail != nullptr)
            tail->next = nullptr;
        else
            head = nullptr;

        delete temp;
        --size;
        return true;
    }

    bool IsEmpty() const
    {
        return size == 0;
    }

    int GetSize() const
    {
        return size;
    }

    void Clear()
    {
        Node<T>* current = head;
        while (current != nullptr)
        {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void Print() const
    {
        if (head == nullptr)
        {
            std::wcout << L"Очередь пуста.\n";
            return;
        }

        Node<T>* current = head;
        while (current != nullptr)
        {
            std::wcout << L"  -> " << current->data.ToString() << L"\n";
            current = current->next;
        }
    }
};

class Cashier
{
private:
    int cashierId;
    bool busy;
    Client currentClient;
    int timeLeft;

public:
    Cashier(int id = 0) : cashierId(id), busy(false), currentClient(), timeLeft(0) {}

    int GetId() const { return cashierId; }
    bool IsBusy() const { return busy; }
    int GetTimeLeft() const { return timeLeft; }

    void AssignClient(const Client& client)
    {
        currentClient = client;
        timeLeft = client.GetServiceTime();
        busy = true;
    }

    bool ProcessOneSecond()
    {
        if (!busy)
            return false;

        --timeLeft;

        if (timeLeft <= 0)
        {
            busy = false;
            return true;
        }

        return false;
    }

    Client GetCurrentClient() const
    {
        return currentClient;
    }

    void CancelCurrentClient()
    {
        busy = false;
        timeLeft = 0;
    }

    std::wstring GetStatus() const
    {
        if (!busy)
        {
            return L"Касса #" + std::to_wstring(cashierId) + L": свободна";
        }

        return L"Касса #" + std::to_wstring(cashierId) +
            L": обслуживает " + currentClient.ToString() +
            L", осталось " + std::to_wstring(timeLeft) + L" сек.";
    }
};

class ShopGame
{
private:
    DoublyLinkedList<Client> queue;
    std::vector<Cashier> cashiers;

    int totalClients = 0;
    int servedClients = 0;
    int nextClientId = 1;
    int timeTick = 0;

public:
    void CreateClients(int count)
    {
        totalClients = count;
        servedClients = 0;
        nextClientId = 1;
        timeTick = 0;
        queue.Clear();
        cashiers.clear();

        for (int i = 0; i < count; ++i)
        {
            int items = rng.NextInt(1, 15);
            int serviceTime = rng.NextInt(1, 3);

            Client client(nextClientId++, items, serviceTime);
            queue.PushBack(client);
        }
    }

    void CreateCashiers(int count)
    {
        cashiers.clear();
        for (int i = 0; i < count; ++i)
        {
            cashiers.emplace_back(i + 1);
        }
    }

    void AssignClientsToFreeCashiers()
    {
        for (auto& cashier : cashiers)
        {
            if (!cashier.IsBusy())
            {
                Client client;
                if (queue.PopFront(client))
                {
                    cashier.AssignClient(client);
                }
            }
        }
    }

    void ProcessTick()
    {
        ++timeTick;

        std::wcout << L"\n========================================\n";
        std::wcout << L"Тик времени: " << timeTick << L"\n";
        std::wcout << L"========================================\n";

        AssignClientsToFreeCashiers();

        for (auto& cashier : cashiers)
        {
            if (cashier.IsBusy())
            {
                Client current = cashier.GetCurrentClient();
                bool finished = cashier.ProcessOneSecond();

                if (finished)
                {
                    ++servedClients;
                    std::wcout << L"[ЗАВЕРШЕНО] "
                        << current.ToString()
                        << L" обслужен на кассе #"
                        << cashier.GetId() << L"\n";
                }
            }
        }
    }

    void OpenNewCashier()
    {
        int newId = static_cast<int>(cashiers.size()) + 1;
        cashiers.emplace_back(newId);
        std::wcout << L"Открыта новая касса #" << newId << L"!\n";
    }

    void CloseLastCashier()
    {
        if (cashiers.empty())
        {
            std::wcout << L"Нет касс для закрытия.\n";
            return;
        }

        Cashier& lastCashier = cashiers.back();

        if (lastCashier.IsBusy())
        {
            Client client = lastCashier.GetCurrentClient();
            queue.PushFront(client);
            std::wcout << L"Клиент с закрываемой кассы возвращён в начало очереди: "
                << client.ToString() << L"\n";
        }

        std::wcout << L"Касса #" << lastCashier.GetId() << L" закрыта.\n";
        cashiers.pop_back();
    }

    void GalyaOtmena()
    {
        if (cashiers.empty())
        {
            std::wcout << L"Касс нет.\n";
            return;
        }

        std::vector<Client> cancelledClients;

        for (auto& cashier : cashiers)
        {
            if (cashier.IsBusy())
            {
                cancelledClients.push_back(cashier.GetCurrentClient());
                cashier.CancelCurrentClient();
            }
        }

        for (int i = static_cast<int>(cancelledClients.size()) - 1; i >= 0; --i)
        {
            queue.PushFront(cancelledClients[i]);
        }

        std::wcout << L"Галя, отмена! Все текущие покупки отменены.\n";
        std::wcout << L"Клиенты возвращены в очередь: " << cancelledClients.size() << L"\n";
    }

    bool IsFinished() const
    {
        if (!queue.IsEmpty())
            return false;

        for (const auto& cashier : cashiers)
        {
            if (cashier.IsBusy())
                return false;
        }

        return true;
    }

    void PrintState() const
    {
        std::wcout << L"\n========== СОСТОЯНИЕ МАГАЗИНА ==========\n";
        std::wcout << L"Всего клиентов: " << totalClients << L"\n";
        std::wcout << L"Обслужено: " << servedClients << L"\n";
        std::wcout << L"Осталось в очереди: " << queue.GetSize() << L"\n";
        std::wcout << L"Количество касс: " << cashiers.size() << L"\n";

        std::wcout << L"\n--- Очередь ---\n";
        queue.Print();

        std::wcout << L"\n--- Кассы ---\n";
        if (cashiers.empty())
        {
            std::wcout << L"Касс нет.\n";
        }
        else
        {
            for (const auto& cashier : cashiers)
            {
                std::wcout << cashier.GetStatus() << L"\n";
            }
        }

        std::wcout << L"========================================\n";
    }

    void Run()
    {
        while (!IsFinished())
        {
            PrintState();

            std::wcout << L"\nВыберите действие:\n";
            std::wcout << L"1 - Следующий тик (1 секунда)\n";
            std::wcout << L"2 - Открыть новую кассу\n";
            std::wcout << L"3 - Закрыть последнюю кассу\n";
            std::wcout << L"4 - \"Галя, отмена!\"\n";
            std::wcout << L"5 - Автозапуск до конца\n";
            std::wcout << L"0 - Выход\n";
            std::wcout << L"> ";

            int command;
            std::wcin >> command;

            switch (command)
            {
            case 1:
                ProcessTick();
                break;

            case 2:
                OpenNewCashier();
                break;

            case 3:
                CloseLastCashier();
                break;

            case 4:
                GalyaOtmena();
                break;

            case 5:
                while (!IsFinished())
                {
                    ProcessTick();
                    PrintState();
                    std::this_thread::sleep_for(std::chrono::milliseconds(700));
                }
                break;

            case 0:
                std::wcout << L"Выход из программы.\n";
                return;

            default:
                std::wcout << L"Неверная команда.\n";
                break;
            }
        }

        std::wcout << L"\nВсе клиенты обслужены. Игра завершена!\n";
    }
};

int InputInRange(const std::wstring& message, int minValue, int maxValue)
{
    int value;
    while (true)
    {
        std::wcout << message;
        std::wcin >> value;

        if (!std::wcin.fail() && value >= minValue && value <= maxValue)
            return value;

        std::wcin.clear();
        std::wcin.ignore(10000, L'\n');
        std::wcout << L"Ошибка ввода. Повторите.\n";
    }
}

int main()
{
    SetupConsole();

    std::wcout << L"========================================\n";
    std::wcout << L"      ИГРА: ОЧЕРЕДЬ В МАГАЗИНЕ\n";
    std::wcout << L"========================================\n";

    ShopGame game;

    std::wcout << L"\nКак задать количество клиентов и касс?\n";
    std::wcout << L"1 - Ввести вручную\n";
    std::wcout << L"2 - Сгенерировать случайно\n";
    std::wcout << L"> ";

    int mode;
    std::wcin >> mode;

    int clientsCount = 0;
    int cashiersCount = 0;

    if (mode == 1)
    {
        clientsCount = InputInRange(L"Введите количество клиентов (5..15): ", 5, 15);
        cashiersCount = InputInRange(L"Введите количество касс (1..10): ", 1, 10);
    }
    else
    {
        clientsCount = rng.NextInt(5, 15);
        cashiersCount = rng.NextInt(1, 5);

        std::wcout << L"\nСлучайно выбрано:\n";
        std::wcout << L"Клиентов: " << clientsCount << L"\n";
        std::wcout << L"Касс: " << cashiersCount << L"\n";
    }

    game.CreateClients(clientsCount);
    game.CreateCashiers(cashiersCount);
    game.Run();

    std::wcout << L"\nНажмите Enter для выхода...";
    std::wcin.ignore(10000, L'\n');
    std::wcin.get();

    return 0;
}