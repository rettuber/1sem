#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

class Student {
public:
    int id;
    string fio;
    double avg;
    string specialty;

    void input() {
        cout << "Введите ID: ";
        cin >> id;
        cin.ignore();

        cout << "Введите ФИО: ";
        getline(cin, fio);

        cout << "Введите средний балл: ";
        cin >> avg;
        cin.ignore();

        cout << "Введите специальность: ";
        getline(cin, specialty);
    }

    void print() {
        cout << "ID: " << id << endl;
        cout << "ФИО: " << fio << endl;
        cout << "Средний балл: " << avg << endl;
        cout << "Специальность: " << specialty << endl;
        cout << "----------------------" << endl;
    }
};

const string filename = "students.txt";

void addStudent() {
    Student s;
    s.input();

    ofstream file(filename, ios::app);
    file << s.id << "|" << s.fio << "|" << s.avg << "|" << s.specialty << endl;
    file.close();

    cout << "Студент добавлен!\n";
}

void showStudents() {
    ifstream file(filename);
    if (!file) {
        cout << "Файл не найден.\n";
        return;
    }

    Student s;
    string line;

    while (getline(file, line)) {
        size_t p1 = line.find("|");
        size_t p2 = line.find("|", p1 + 1);
        size_t p3 = line.find("|", p2 + 1);

        s.id = stoi(line.substr(0, p1));
        s.fio = line.substr(p1 + 1, p2 - p1 - 1);
        s.avg = stod(line.substr(p2 + 1, p3 - p2 - 1));
        s.specialty = line.substr(p3 + 1);

        s.print();
    }

    file.close();
}

void searchStudent() {
    ifstream file(filename);
    if (!file) {
        cout << "Файл не найден.\n";
        return;
    }

    int choice;
    cout << "Поиск по:\n";
    cout << "1 - ID\n";
    cout << "2 - ФИО\n";
    cout << "3 - Среднему баллу\n";
    cout << "4 - Специальности\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    cin.ignore();

    string key;
    cout << "Введите значение: ";
    getline(cin, key);

    Student s;
    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t p1 = line.find("|");
        size_t p2 = line.find("|", p1 + 1);
        size_t p3 = line.find("|", p2 + 1);

        s.id = stoi(line.substr(0, p1));
        s.fio = line.substr(p1 + 1, p2 - p1 - 1);
        s.avg = stod(line.substr(p2 + 1, p3 - p2 - 1));
        s.specialty = line.substr(p3 + 1);

        if ((choice == 1 && to_string(s.id) == key) ||
            (choice == 2 && s.fio == key) ||
            (choice == 3 && to_string(s.avg) == key) ||
            (choice == 4 && s.specialty == key)) {
            s.print();
            found = true;
        }
    }

    if (!found)
        cout << "Ничего не найдено.\n";

    file.close();
}

void deleteStudent() {
    int delId;
    cout << "Введите ID для удаления: ";
    cin >> delId;

    ifstream file(filename);
    ofstream temp("temp.txt");

    Student s;
    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t p1 = line.find("|");
        int id = stoi(line.substr(0, p1));

        if (id != delId)
            temp << line << endl;
        else
            found = true;
    }

    file.close();
    temp.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        cout << "Запись удалена.\n";
    else
        cout << "Студент не найден.\n";
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice;

    while (true) {
        cout << "\n===== База данных студентов ИТМО =====\n";
        cout << "1. Добавить студента\n";
        cout << "2. Показать всех студентов\n";
        cout << "3. Поиск\n";
        cout << "4. Удалить по ID\n";
        cout << "0. Выход\n";
        cout << "Выберите пункт: ";
        cin >> choice;

        switch (choice) {
        case 1: addStudent(); break;
        case 2: showStudents(); break;
        case 3: searchStudent(); break;
        case 4: deleteStudent(); break;
        case 0: return 0;
        default: cout << "Неверный пункт\n";
        }
    }
}