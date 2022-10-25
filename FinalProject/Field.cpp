#include <iostream> 
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include "Header.h"
#define MAP_SIZE 10

using namespace std;

void Clean()
{
    COORD coord{0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void ConsoleCursorVisible(bool visible) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

int checkCoord(const int& coord) {
    if (coord < 0 || coord >= MAP_SIZE) {
        throw "Error";
    }
    return coord;
}

void setVerifyType(Field& Field, int y, int x, int value) {
    if (x < 0 || x >= 10 || y < 0 || y >= 10) {
        return;
    }
    Field.cells[checkCoord(y)][checkCoord(x)].verifyType = value;
}

void setType(Field& Field, int y, int x, int value) {
    if (x < 0 || x >= 10 || y < 0 || y >= 10) {
        return;
    }
    Field.cells[checkCoord(y)][checkCoord(x)].type = value;
}

void fieldPrint(Field& Field, const int xMove, const int yMove, int y, bool isBot, int x, ConsoleColor color) {
    string numeric = "ABCDEFGHIJ";
    int i, j;
    cout << endl;
    for (i = 0; i < MAP_SIZE; i++) {
        cout << "  ";
        for (j = 0; j < MAP_SIZE; j++) {
            if (Field.cells[i][j].type == AIM) {             // Прицел
                SetColor(BLACK, RED_FADE);
                SetCursorPosition((x + j) * 2, y + i);
                cout << "><";
                continue;
            }
            else if (Field.cells[i][j].verifyType == DESTROY_SHIP) {  // Зруйнованана частина корабля
                SetColor(RED_FADE, RED_FADE);
            }
            else if (Field.cells[i][j].verifyType == MISS) {  // Мимо
                SetColor(TEAL_FADE, TEAL_FADE);
            }
            else if (Field.cells[i][j].type == SHIP_PLACE) {
                SetColor(GRAY, GRAY);
            }
            else if (Field.cells[i][j].verifyType == AROUND_DESTROY) {
                SetColor(TEAL_FADE, TEAL_FADE);
            }
            else if (Field.cells[i][j].verifyType == AROUND && !isBot) {
                SetColor(BLUE, BLUE);
            }
            else if (Field.cells[i][j].verifyType >= SHIP && !isBot) {
                SetColor(GREEN_FADE, GREEN_FADE);
            }
            else if (Field.cells[i][j].type == EMPTY) {        // Пуста клітинка при розстановці
                SetColor(BLUE_FADE, BLUE_FADE);
            }
            SetCursorPosition((x + j) * 2, y + i);
            cout << "**";
        }
        SetColor(BLACK, BLACK);
        cout << "00" << endl;
        SetColor(GRAY, BLACK);
    }
    
    SetColor(color, BLACK);

    for (i = 0; i < MAP_SIZE; i++) {
        SetCursorPosition(x * 2 - 2, y + i);
        cout << i + 1;
        SetCursorPosition((x + i) * 2, y - 1);
        cout << numeric[i];
    }
}

void CellAroundInsert(Field& Field, int y, int x, int value) {
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (Field.cells[i][j].verifyType >= SHIP || Field.cells[i][j].verifyType == DESTROY_SHIP) continue;
            setVerifyType(Field, i, j, value);
        }
    }
}

bool shipAround(Field& Field, int y, int x) {
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (i < 0 || j < 0) continue;
            if ((Field.cells[i][j].verifyType >= SHIP)) return false;
        }
    }
    return true;
}

bool dirSelect(Field& Field, int y, int x) {
    if (Field.cells[y + 1][x].verifyType >= SHIP || Field.cells[y + 1][x].verifyType == DESTROY_SHIP ||
        Field.cells[y - 1][x].verifyType >= SHIP || Field.cells[y - 1][x].verifyType == DESTROY_SHIP) return true;
    return false;
}

void fieldAroundInsert(Field& Field, int y, int x, int value) {
    bool vertical = dirSelect(Field, y, x);    
    while (((Field.cells[y][x - 1].verifyType >= SHIP || Field.cells[y][x - 1].verifyType == DESTROY_SHIP) && !vertical)
        || ((Field.cells[y - 1][x].verifyType >= SHIP || Field.cells[y - 1][x].verifyType == DESTROY_SHIP) && vertical)) {
        if (vertical) y--;
        else x--;
    }

    while (Field.cells[y][x].verifyType >= SHIP || Field.cells[y][x].verifyType == DESTROY_SHIP) {
        CellAroundInsert(Field, y, x, value);
        if (vertical) y++;
        else x++;
    }
}

void enterVerifyType(Field& Field, int y, int x, int shipSize, bool vertical) {
    int i;
    for (i = 0; i < shipSize; i++) {
        if (vertical) {
            setType(Field, y + i, x, EMPTY);
            setVerifyType(Field, y + i, x, SHIP + Field.shipCount);
        }
        else {
            setType(Field, y, x + i, EMPTY);
            setVerifyType(Field, y, x + i, SHIP + Field.shipCount);
        }
    }
}

void autoShipPlacing(Field& Field, const int shipSize) {
    int i, x{-1}, y{-1};
    bool vertical{ true }, insertedPossible;
    start: while (true) {
        insertedPossible = true;
        x = rand() % MAP_SIZE;
        y = rand() % MAP_SIZE;
        if (x + shipSize >= MAP_SIZE) goto start;
        if (y + shipSize >= MAP_SIZE) goto start;
        vertical = rand() % 2;
        for (i = 0; i < shipSize; i++) {
            if (vertical) {
                if (!shipAround(Field, y + i, x)) goto start;
            }
            else {
                if (!shipAround(Field, y, x + i)) goto start;
            }
        }
        enterVerifyType(Field, y, x, shipSize, vertical);
        fieldAroundInsert(Field, y, x, AROUND);
        // Інкремент щоб у кожного корабля був свій індекс
        Field.shipCount++;
        break;
    }
}

void autoFieldPlacing(Field& Field) {
    for (int i = 0; i < 10; i++) {
        autoShipPlacing(Field, Field.ships[i]);
    }
}

void moveShip(bool& vertical, int& x, int& y, const int shipSize, const int key) {

    switch (key) {
    case UP_ARROW:          // Здвиг верх
        if (y - 1 == -1) break;
        y--;
        break;

    case DOWN_ARROW:        // Здвиг вниз
        if ((y + 1 == MAP_SIZE && !vertical) || (y + shipSize == MAP_SIZE && vertical)) break;
        y++;
        break;

    case LEFT_ARROW:        // Здвиг вліво
        if (x - 1 == -1) break;
        x--;
        break;

    case RIGHT_ARROW:       // Здвиг вправо
        if ((x + shipSize == MAP_SIZE && !vertical) || (x + 1== MAP_SIZE && vertical)) break;
        x++;
        break;

    case SPACE:
        if (vertical && x + shipSize > MAP_SIZE) break;
        else if (!vertical && y + shipSize > MAP_SIZE) break;
        vertical = !vertical;
    default: break;
    }
}

void changeTypePosition(Field& Field, int shipSize, bool vertical, int x, int y, int value) {
    int i;
    for (i = 0; i < shipSize; i++) {
        if (vertical) {
            setType(Field, y + i, x, value);
        }
        else {
            setType(Field, y, x + i, value);
        }
    }
}

void shipPlace(Field& Field, int shipSize, Settings settings) {
    int i, x{ 0 }, y{ 0 }, key;
    bool vertical{ true }, insertedPossible{ true };
    start: while (true) {
        fieldPrint(Field, x, y, Y_ALL, false, X_PLAYER_PLACE, settings.color);
        key = _getch();
        if (key == ENTER) {
            for (i = 0; i < shipSize; i++) {
                if (vertical) {
                    if (!shipAround(Field, y + i, x)) goto start;
                }
                else {
                    if (!shipAround(Field, y, x + i)) goto start;
                }
            }
            enterVerifyType(Field, y, x, shipSize, vertical);
            fieldAroundInsert(Field, y, x, AROUND);
            break;
        }
        changeTypePosition(Field, shipSize, vertical, x, y, EMPTY);
        moveShip(vertical, x, y, shipSize, key);
        changeTypePosition(Field, shipSize, vertical, x, y, SHIP_PLACE);
    }
}

void userFieldPlacing(Field& Field, Settings settings) {
    int i;
    for (i = 0; i < 10; i++) {
        shipPlace(Field, Field.ships[i], settings);
    }
}
