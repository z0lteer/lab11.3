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
        cout << "������� �������� ����� ��� ������.\n";
        return;
    }

    Note person;
    cout << "������ �������: ";
    cin >> person.surname;
    cout << "������ ��'�: ";
    cin >> person.name;
    cout << "������ ����� ��������: ";
    cin >> person.phone_number;


    cout << "������ ���� ���������� (����, �����, ��): ";
    while (true) {
        cin >> person.birthday[0] >> person.birthday[1] >> person.birthday[2];
        if (person.birthday[0] > 0 && person.birthday[0] <= 31 &&
            person.birthday[1] > 0 && person.birthday[1] <= 12 &&
            person.birthday[2] > 1900 && person.birthday[2] <= 2100) {
            break;
        }
        cout << "�������: ������ �������� ���� � ������ DD MM YYYY.\n";
    }


    file << setw(15) << left << person.surname
        << setw(10) << left << person.name
        << setw(15) << left << person.phone_number
        << setw(3) << person.birthday[0] << " "
        << setw(3) << person.birthday[1] << " "
        << setw(5) << person.birthday[2] << "\n";

    cout << "��� ������ ������ �� �����.\n";
    file.close();
}



void searchBySurname(const string& filename, const string& surname) {
    ifstream file(filename);
    if (!file) {
        cout << "������� �������� ����� ��� �������.\n";
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
            cout << "�������: " << person.surname
                << ", ��'�: " << person.name
                << ", ����� ��������: " << person.phone_number
                << ", ���� ����������: " << person.birthday[0] << "."
                << person.birthday[1] << "." << person.birthday[2] << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "������ � �������� " << surname << " �� ��������.\n";
    }

    file.close();
}


void sortByPhoneNumber(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "������� �������� ����� ��� �������.\n";
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
        cout << "������� �������� ����� ��� ������.\n";
        delete[] people;
        return;
    }

    for (int i = 0; i < size; ++i) {
        outFile << people[i].surname << " " << people[i].name << " " << people[i].phone_number << " "
            << people[i].birthday[0] << " " << people[i].birthday[1] << " " << people[i].birthday[2] << "\n";
    }

    delete[] people;
    outFile.close();
    cout << "������ ������ ����������� �� ������� ��������.\n";
}


void displayMenu() {
    cout << "\n--- ���� ---\n";
    cout << "1. ������ ��� ��� ������\n";
    cout << "2. ����� �� ��������\n";
    cout << "3. ��������� �� ������� ��������\n";
    cout << "4. �����\n";
    cout << "������� �����: ";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    cout << "������ ��'� ����� ��� ������ (���������, data.txt): ";
    cin >> filename;


    if (!fileExists(filename)) {
        ofstream createFile(filename);
        if (!createFile) {
            cout << "������� ��������� �����.\n";
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
            cout << "������ ������� ��� ������: ";
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
            cout << "������� ����. ��������� �� ���.\n";
        }
    }

    return 0;
}