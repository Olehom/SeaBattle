#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Header.h"
#define MAP_SIZE 10

using namespace std;

void actionDescription(int x, int y, int action, ConsoleColor color) {
    SetColor(color, BLACK);
    SetCursorPosition(x, y);
    cout << "                                                   ";
    SetCursorPosition(x, y);
    cout << "Дія: ";
    switch (action) {
    case PLAYER_MISS: cout << "Ви нікуди не попали!"; break;
        Sleep(250); break;
    case BOT_MISS: cout << "Бот нікуди не попав!"; break;
        Sleep(250); break;
    case PLAYER_DECK_DESTROY: cout << "Бот зруйнував вашу палубу!";
        Sleep(750); break;
    case PLAYER_SHIP_DESTROY: cout << "Бот зруйнував ваш корабель!";
        Sleep(750); break;
    case BOT_DECK_DESTROY: cout << "Ви зруйнували палубу бота!"; break;
    case BOT_SHIP_DESTROY: cout << "Ви зруйнували корабель бота!"; break;
    default: break;
    }
    SetColor(BLACK, BLACK);
}

bool botHint(Field& Field, int y, int x) {
    if (Field.cells[y][x].verifyType >= SHIP) {
        return true;
    }
    else {
        Field.cells[y][x].verifyType = MISS;
        return false;
    }
}

bool botAttack(Field& Field, int& action, int& xReturn, int& yReturn) {
    int x, y;
    while (true) {
        x = rand() % MAP_SIZE;
        y = rand() % MAP_SIZE;
        if (Field.cells[y][x].verifyType == MISS || Field.cells[y][x].verifyType == DESTROY_SHIP) continue;

        if (botHint(Field, y, x)) {
            Field.ships[Field.cells[y][x].verifyType - SHIP]--;
            if (Field.ships[Field.cells[y][x].verifyType - SHIP] == 0) {
                fieldAroundInsert(Field, y, x, AROUND_DESTROY);
                Field.cells[y][x].verifyType = DESTROY_SHIP;
                xReturn = x;
                yReturn = y;
                return true;
            }
            Field.cells[y][x].verifyType = DESTROY_SHIP;
            return true;
        }
        else {
            action = BOT_MISS;
            return false;
        }
    }
}

bool playerAttack(Field& botField, int& action, Settings settings, Field& playerField, User user) {
    int dir{0}, key, x{0}, y{0}, shipSize{0};
    while (true) {
        fieldPrint(botField, -1, -1, Y_ALL, true, X_BOT_FIGHT, settings.color);
        key = _getch();
        botField.cells[y][x].type = EMPTY;
        switch (key) {
        case ENTER:
            if (botField.cells[y][x].verifyType == MISS || botField.cells[y][x].verifyType == DESTROY_SHIP) continue;

            if (botField.cells[y][x].verifyType >= SHIP) {
                botField.ships[botField.cells[y][x].verifyType - SHIP]--;
                if (botField.ships[botField.cells[y][x].verifyType - SHIP] == 0) {
                    fieldAroundInsert(botField, y, x, AROUND_DESTROY);
                    action = BOT_SHIP_DESTROY;
                }
                else action = BOT_DECK_DESTROY;
                botField.cells[y][x].verifyType = DESTROY_SHIP;
                return true;
            }
            else {
                botField.cells[y][x].verifyType = MISS;
                action = PLAYER_MISS;
                return false;
            }
            break;
        case UP_ARROW: 
            if(y - 1 != -1) y--;
            break;
        case DOWN_ARROW:
            if (y + 1 != MAP_SIZE) y++;
            break;
        case LEFT_ARROW: 
            if (x - 1 != -1) x--;
            break;
        case RIGHT_ARROW:
            if (x + 1 != MAP_SIZE) x++;
            break;
        case ESC:
            saveUserGame(user, playerField, botField);
            exit(0);
            break;
        default: break;
        }
        botField.cells[y][x].type = AIM;
    }
}

bool isShips(int* ships) {
    for (int i = 0; i < MAP_SIZE; i++) {
        if (ships[i] != 0) {
            return false;
        }
    }
    return true;
}

void difficultyChoiceAttack(Settings settings, Field& Field, AI& Ai, int& action, bool& step) {
    int x, y;
    if (settings.difficulty) {
        step = !step;
        if (AIAttack(Field, Ai, action)) {
            if (Ai.decksCount == 0) {
                fieldAroundInsert(Field, Ai.hintY, Ai.hintX, AROUND_DESTROY);
            }
            step = !step;
        }
    }
    else {
        step = !step;
        if (botAttack(Field, action,x, y)) {
            if (isShips(Field.ships)) {
                fieldAroundInsert(Field, x, y, AROUND_DESTROY);
            }
            step = !step;
        }
    }
}

void winAction(Field Field1, Field Field2, Settings settings, string winName) {
    fieldPrint(Field1, -1, -1, Y_ALL, false, X_PLAYER_FIGHT, settings.color);
    fieldPrint(Field2, -1, -1, Y_ALL, false, X_BOT_FIGHT, settings.color);
    SetCursorPosition(X_MENU * 2.3, Y_ALL - 3);
    SetColor(BLACK, settings.color);
    cout << winName;
    _getch();
    system("cls");
}

void botPlay(Field& botField1, Field& botField2, Settings settings) {
    AI Ai1;
    AI Ai2;
    string name;
    int action, bot1Ships{ 20 }, bot2Ships{ 20 };
    bool step = true;
    while (!isShips(botField1.ships) && !isShips(botField2.ships)) {
        fieldPrint(botField1, -1, -1, Y_ALL, false, X_PLAYER_FIGHT, settings.color);
        fieldPrint(botField2, -1, -1, Y_ALL, false, X_BOT_FIGHT, settings.color);
        if (step) {
            difficultyChoiceAttack(settings, botField1, Ai1, action, step);
        }
        else {
            difficultyChoiceAttack(settings, botField2, Ai2, action, step);
        }
        Sleep(75);
    }
    name = !isShips(botField2.ships) ? "Переміг другий бот" : "Переміг перший бот";
    winAction(botField1, botField2, settings, name);
}

void play(Field& playerField, Field& enemyField, Settings settings, User& user) {
    int i, j;
    AI Ai;
    string name;
    system("cls");
    int action;
    bool step{ true }, winner;
    while (!isShips(playerField.ships) && !isShips(enemyField.ships)) {
        fieldPrint(playerField, -1, -1, Y_ALL, false, X_PLAYER_FIGHT, settings.color);
        fieldPrint(enemyField, -1, -1, Y_ALL, true, X_BOT_FIGHT, settings.color);
        if (step) {
            step = !step;
            if (playerAttack(enemyField, action, settings, playerField, user)) {
                step = !step;
                user.shipsDestroyer++;
            }
        }
        else {
            difficultyChoiceAttack(settings, playerField, Ai, action, step);
        }
        actionDescription(X_MESSAGES * 2, Y_MESSAGES, action, settings.color);
    }
    if (isShips(playerField.ships)) {
        name = "Переміг бот";
    }
    else {
        name = "Ви перемогли";
        user.wins++;
    }
    user.games++;
    winAction(playerField, enemyField, settings, name);
}
