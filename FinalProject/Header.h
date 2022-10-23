#pragma once
#include <string>

using std::string;

enum Actions {
    PLAYER_MISS = 0,
    BOT_MISS = 1,
    PLAYER_DECK_DESTROY = 2,
    PLAYER_SHIP_DESTROY = 3,
    BOT_DECK_DESTROY = 4,
    BOT_SHIP_DESTROY = 5,
};

// Enums
enum ConsoleColor {
    // Standard text colors
    GRAY = 8, BLUE, GREEN,
    TEAL, RED, PINK,
    YELLOW, WHITE,
    // Faded text colors
    BLACK = 0, BLUE_FADE, GREEN_FADE,
    TEAL_FADE, RED_FADE, PINK_FADE,
    YELLOW_FADE, WHITE_FADE,
};

enum Directions {
    DOWN = 0, LEFT = 1, UP = 2, RIGHT = 3,
};

enum Keys {
    ESC = 27, ENTER = 13, UP_ARROW = 72,
    DOWN_ARROW = 80, LEFT_ARROW = 75,
    RIGHT_ARROW = 77, PLUS = 43, MINUS = 45,
    SPACE = 32
};

enum Cells {
    EMPTY = 0, SHIP_PLACE = 1, AIM = 2, 
    DESTROY_SHIP = 1, MISS = 2, AROUND = 3,
    AROUND_DESTROY = 4, SHIP = 5,
};

enum Positions {        // Можливі позиції полів. Це зручно, тому-що можна легко регулювати розміщення певних елементів.
    Y_ALL = 8,
    Y_WIN = 12,
    Y_MESSAGES = 19,
    X_PLAYER_FIGHT = 20,
    X_MENU = 24,
    X_PLAYER_PLACE = 24,
    X_MESSAGES = 26,
    X_BOT_FIGHT = 35,
    X_WIN = 25,
};

struct Settings {
    char graphUnit;
    bool difficulty;
    ConsoleColor color;
};

struct User {
    string login;
    string password;
    char graphUnit;
    bool difficulty;
    int color;
    int wins;
    int games;
    int shipsDestroyer;
    int percentOfWin;
};

struct AI {
    int hintX{ -1 };  // Перше попадання по X
    int hintY{ -1 };  // Перше попадання по Y
    int decksCount{ 0 };
    int dir{ 0 };     // Напрямок наступного пострілу
    int x{ -1 };      // Наступний постріл по x
    int y{ -1 };      // Наступний постріл по y
};

struct Cell {
    int type = 0;         // Змінна клітинка
    int verifyType = 0;   // Незмінна клітинка
};

struct Field {
    Cell cells[10][10];                     // Масив клітинок
    int ships[10]{ 4,3,3,2,2,2,1,1,1,1 };
    int shipCount{ 0 };
};

// Functions
void SetCursorPosition(short x, short y);
void SetColor(ConsoleColor text, ConsoleColor background);
void Cyrrilic();
void fieldPrint(Field& Field, const int xMove, const int yMove, int y, bool isBot, int x, ConsoleColor color);
void userFieldPlacing(Field& Field, Settings settings);
void play(Field& playerField, Field& enemyField, Settings settings, User& user);
void autoFieldPlacing(Field& Field);
void ConsoleCursorVisible(bool visible);
void fieldAroundInsert(Field& Field, int y, int x, int value);
bool AIAttack(Field& Field, AI& Ai, int& action);
bool botHint(Field& Field, int y, int x);
int startMenu(const int x, const int y, int menuSize, int arrSize, string* MenuParagraph, string Name, Settings settings);
void MenuPrint(const int SIZE, const int x, const int y, const char GraphUnit, string Name, ConsoleColor Color);
void StatisticMenu(const int x, const int y, int menuSize, User user, Settings settings);
int changeColor(Settings settings);
void botPlay(Field& botField1, Field& botField2, Settings settings);
void saveUserGame(const User& user, Field Field1, Field Field2);
void loadUserGame(const User& user, Field& Field1, Field& Field2);
char changeChar(Settings settings);
void settingsToDefault(User& user);
void saveUser(const User& user); 
bool gameExists(const User& user);
User loginUser();
User registerNewUser();
int checkCoord(const int& coord);
bool userExists(const User& user);
