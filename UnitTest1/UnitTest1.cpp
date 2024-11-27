#include "pch.h"
#include "CppUnitTest.h"


#include <fstream>
#include "../Lab11.3/lab11.1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestAddPersonToFile)
        {
            const string testFilename = "test_add_person.txt";


            ofstream outFile(testFilename, ios::trunc);
            outFile.close();

            std::istringstream input("Doe\nJohn\n555-6789\n10 10 1990\n");
            std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
            addPersonToFile(testFilename);
            std::cin.rdbuf(oldCin);


            ifstream inFile(testFilename);
            Assert::IsTrue(inFile.good(), L"Файл повинен існувати.");

            string line;
            getline(inFile, line);
            inFile.close();

            Assert::IsTrue(line.find("Doe") != string::npos, L"Дані про людину мають бути записані у файл.");
        }

        TEST_METHOD(TestSearchBySurname_Found)
        {
            const string testFilename = "test_search_found.txt";


            ofstream outFile(testFilename, ios::trunc);
            outFile << setw(15) << left << "Ivanov" << setw(10) << left << "Ivan"
                << setw(15) << left << "555-1234" << setw(3) << 1 << " " << setw(3) << 1 << " " << setw(5) << 1990 << "\n";
            outFile.close();


            std::ostringstream oss;
            std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

            searchBySurname(testFilename, "Ivanov");

            std::cout.rdbuf(oldCout);
            string output = oss.str();

            Assert::IsTrue(output.find("Ivanov") != string::npos, L"Інформація про прізвище повинна бути виведена.");
        }

        TEST_METHOD(TestSearchBySurname_NotFound)
        {
            const string testFilename = "test_search_notfound.txt";


            ofstream outFile(testFilename, ios::trunc);
            outFile << setw(15) << left << "Ivanov" << setw(10) << left << "Ivan"
                << setw(15) << left << "555-1234" << setw(3) << 1 << " " << setw(3) << 1 << " " << setw(5) << 1990 << "\n";
            outFile.close();


            std::ostringstream oss;
            std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

            searchBySurname(testFilename, "Nonexistent");

            std::cout.rdbuf(oldCout);
            string output = oss.str();

            Assert::IsTrue(output.find("не знайдена") != string::npos, L"Повідомлення про ненайдену людину повинно бути виведено.");
        }

        TEST_METHOD(TestSortByPhoneNumber)
        {
            const string testFilename = "test_sort.txt";


            ofstream outFile(testFilename, ios::trunc);
            outFile << setw(15) << left << "Ivanov" << setw(10) << left << "Ivan"
                << setw(15) << left << "555-1234" << setw(3) << 1 << " " << setw(3) << 1 << " " << setw(5) << 1990 << "\n";
            outFile << setw(15) << left << "Petrov" << setw(10) << left << "Petr"
                << setw(15) << left << "555-0123" << setw(3) << 2 << " " << setw(3) << 2 << " " << setw(5) << 1991 << "\n";
            outFile << setw(15) << left << "Sidorov" << setw(10) << left << "Sidr"
                << setw(15) << left << "555-5678" << setw(3) << 3 << " " << setw(3) << 3 << " " << setw(5) << 1992 << "\n";
            outFile.close();


            sortByPhoneNumber(testFilename);


            ifstream inFile(testFilename);
            Assert::IsTrue(inFile.good(), L"Файл повинен існувати.");

            string firstLine, secondLine, thirdLine;
            getline(inFile, firstLine);
            getline(inFile, secondLine);
            getline(inFile, thirdLine);
            inFile.close();

            Assert::IsTrue(firstLine.find("555-0123") != string::npos, L"Перший номер телефону повинен бути 555-0123.");
            Assert::IsTrue(secondLine.find("555-1234") != string::npos, L"Другий номер телефону повинен бути 555-1234.");
            Assert::IsTrue(thirdLine.find("555-5678") != string::npos, L"Третій номер телефону повинен бути 555-5678.");
        }
    };
}