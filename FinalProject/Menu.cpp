#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Header.h"

using namespace std;

int i, j;

void Cyrrilic() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}

void SetCursorPosition(short x, short y)
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(ConsoleColor text, ConsoleColor background)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (short)text | ((short)background << 4));
}

void ActiveOption(const bool isActive, ConsoleColor color) {
    if (isActive) SetColor(BLACK, color);
    else SetColor(WHITE, BLACK);
}

void MenuOptionsPrint(const int SIZE, const int x, const int y, const int option, string* MenuParagraph, const int arrSize, ConsoleColor color) {
    for (i = 0; i < arrSize; i++) {
        ActiveOption(option == i, color);
        SetCursorPosition(SIZE + x - MenuParagraph[i].size() / 2 + 1, SIZE / 2 + y + (-2 + i));
        cout << MenuParagraph[i];
    }
    SetCursorPosition(0,0);
}

void MenuPrint(const int SIZE, const int x, const int y, const char GraphUnit, string Name, ConsoleColor Color) {
    SetCursorPosition(x, y);
    SetColor(BLACK, BLACK);
    SetColor(Color, BLACK);
    for (i = 0; i < SIZE; i++) {    // Border print
        SetCursorPosition(x + (i * 2) + 1, y);              // Up
        cout << GraphUnit;
        SetCursorPosition(x + (i * 2) + 1, y + SIZE - 1);   // Down
        cout << GraphUnit;
        SetCursorPosition(x + 1, y + i);                    // Right
        cout << GraphUnit;
        SetCursorPosition(x + SIZE * 2 + 1, y + i);         // Left
        cout << GraphUnit;
    }
    SetCursorPosition(SIZE + x - Name.size()/2 + 1, y + 1); // User name placing
    cout << Name;
}

char changeChar(Settings settings) {
begin:
    char graphUnit;
    MenuPrint(3, X_MENU * 2.3, Y_ALL, settings.graphUnit, "", settings.color);
    SetCursorPosition(X_MENU * 2, Y_ALL + 4);
    cout << "Введіть будь-який символ";
    SetCursorPosition(X_MENU * 2.4 + 1, Y_ALL + 1);
    cin >> graphUnit;
    return graphUnit;
}

int changeColor(Settings settings) {
    begin:
    int choice;
    MenuPrint(3, X_MENU * 2.3, Y_ALL, settings.graphUnit, "", settings.color);
    SetCursorPosition(X_MENU * 1.8, Y_ALL + 4);
    for (i = 1; i < 16; i++) {
        SetColor((ConsoleColor)i, BLACK);
        cout << i << " ";
    }
    SetCursorPosition(X_MENU * 2.4 + 1, Y_ALL + 1);
    cin >> choice;
    if (choice < 1 || choice > 16) {
        goto begin;
    }
    return choice;
}

void StatisticMenu(const int x, const int y, int menuSize, User user, Settings settings) {
    MenuPrint(menuSize, x, y, settings.graphUnit, "Статистика", settings.color);
    SetCursorPosition(X_MENU * 2.1 + 1, Y_ALL + 3);
    cout << "Ігри: " << user.games;
    SetCursorPosition(X_MENU * 2.1 + 1, Y_ALL + 4);
    cout << "Перемоги: " << user.wins;
    SetCursorPosition(X_MENU * 2.1 + 1, Y_ALL + 5);
    cout << "Відсоток перемог: " << (int)user.percentOfWin << "%";
    SetCursorPosition(X_MENU * 2.1 + 1, Y_ALL + 6);
    cout << "Зруйновані кораблі: " << user.shipsDestroyer;

    SetCursorPosition(X_MENU * 2.1 + 1, Y_ALL + menuSize - 3);
    cout << "Натисніть любу клавішу!";
    _getch();
}

int startMenu(const int x, const int y, int menuSize, int arrSize, string* MenuParagraph, string Name, Settings settings) {
    system("cls");
    int key, activeOption{ 0 };
    MenuPrint(menuSize, x, y, settings.graphUnit, Name, settings.color);        // Display menu without buttons
    MenuOptionsPrint(menuSize, x, y, activeOption, MenuParagraph, arrSize, settings.color);     // Display buttons
    while (true) {
        key = _getch();
        if (key != -1) {
            switch (key) {
            case ENTER:
                system("cls");
                return activeOption;
                break;
            case UP_ARROW:
                if (activeOption > 0) {
                    activeOption--;
                }
                else {
                    activeOption = arrSize - 1;
                }
                MenuOptionsPrint(menuSize, x, y, activeOption, MenuParagraph, arrSize, settings.color);
                break;
            case DOWN_ARROW:
                if (activeOption < arrSize - 1) {
                    activeOption++;
                }
                else {
                    activeOption = 0;
                }
                MenuOptionsPrint(menuSize, x, y, activeOption, MenuParagraph, arrSize, settings.color);
                break;
            default: break;
            }
        }
    }
}
