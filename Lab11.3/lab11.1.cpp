#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <locale> 
using namespace std;

struct Note {
    string surname;
    string name;
    string phone_number;
    int birthday[3];
};


bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}


void addPersonToFile(const string& filename) {
    ofstream file(filename, ios::app);
    if (!file) {
        cout << "Помилка відкриття файлу для запису.\n";
        return;
    }

    Note person;
    cout << "Введіть прізвище: ";
    cin >> person.surname;
    cout << "Введіть ім'я: ";
    cin >> person.name;
    cout << "Введіть номер телефону: ";
    cin >> person.phone_number;


    cout << "Введіть день народження (день, місяць, рік): ";
    while (true) {
        cin >> person.birthday[0] >> person.birthday[1] >> person.birthday[2];
        if (person.birthday[0] > 0 && person.birthday[0] <= 31 &&
            person.birthday[1] > 0 && person.birthday[1] <= 12 &&
            person.birthday[2] > 1900 && person.birthday[2] <= 2100) {
            break;
        }
        cout << "Помилка: введіть коректну дату у форматі DD MM YYYY.\n";
    }


    file << setw(15) << left << person.surname
        << setw(10) << left << person.name
        << setw(15) << left << person.phone_number
        << setw(3) << person.birthday[0] << " "
        << setw(3) << person.birthday[1] << " "
        << setw(5) << person.birthday[2] << "\n";

    cout << "Дані успішно додано до файлу.\n";
    file.close();
}



void searchBySurname(const string& filename, const string& surname) {
    ifstream file(filename);
    if (!file) {
        cout << "Помилка відкриття файлу для читання.\n";
        return;
    }

    locale loc("uk_UA.UTF-8");
    Note person;
    bool found = false;

    string searchSurname = surname;
    for (auto& ch : searchSurname) ch = tolower(ch, loc);

    while (file >> person.surname >> person.name >> person.phone_number
        >> person.birthday[0] >> person.birthday[1] >> person.birthday[2]) {

        string fileSurname = person.surname;
        for (auto& ch : fileSurname) ch = tolower(ch, loc);

        if (fileSurname == searchSurname) {
            cout << "Прізвище: " << person.surname
                << ", Ім'я: " << person.name
                << ", Номер телефону: " << person.phone_number
                << ", День народження: " << person.birthday[0] << "."
                << person.birthday[1] << "." << person.birthday[2] << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "Людина з прізвищем " << surname << " не знайдена.\n";
    }

    file.close();
}


void sortByPhoneNumber(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Помилка відкриття файлу для читання.\n";
        return;
    }


    Note* people = nullptr;
    int size = 0;
    string line;

    while (getline(file, line)) {
        ++size;
    }

    file.clear();
    file.seekg(0);


    people = new Note[size];
    for (int i = 0; i < size; ++i) {
        file >> people[i].surname >> people[i].name >> people[i].phone_number
            >> people[i].birthday[0] >> people[i].birthday[1] >> people[i].birthday[2];
    }

    file.close();


    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (people[j].phone_number > people[j + 1].phone_number) {
                Note temp = people[j];
                people[j] = people[j + 1];
                people[j + 1] = temp;
            }
        }
    }


    ofstream outFile(filename, ios::trunc);
    if (!outFile) {
        cout << "Помилка відкриття файлу для запису.\n";
        delete[] people;
        return;
    }

    for (int i = 0; i < size; ++i) {
        outFile << people[i].surname << " " << people[i].name << " " << people[i].phone_number << " "
            << people[i].birthday[0] << " " << people[i].birthday[1] << " " << people[i].birthday[2] << "\n";
    }

    delete[] people;
    outFile.close();
    cout << "Записи успішно відсортовано за номером телефону.\n";
}


void displayMenu() {
    cout << "\n--- Меню ---\n";
    cout << "1. Додати дані про людину\n";
    cout << "2. Пошук за прізвищем\n";
    cout << "3. Сортувати за номером телефону\n";
    cout << "4. Вийти\n";
    cout << "Виберіть опцію: ";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    cout << "Введіть ім'я файлу для роботи (наприклад, data.txt): ";
    cin >> filename;


    if (!fileExists(filename)) {
        ofstream createFile(filename);
        if (!createFile) {
            cout << "Помилка створення файлу.\n";
            return 1;
        }
        createFile.close();
    }

    bool running = true;
    while (running) {
        displayMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            addPersonToFile(filename);
            break;
        case 2: {
            string surname;
            cout << "Введіть прізвище для пошуку: ";
            cin >> surname;
            searchBySurname(filename, surname);
            break;
        }
        case 3:
            sortByPhoneNumber(filename);
            break;
        case 4:
            running = false;
            break;
        default:
            cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    }

    return 0;
}