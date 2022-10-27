#include <iostream>
#include <Windows.h>
#include <time.h>
#include <string>
#include "Header.h"

using namespace std;

void startGame(Settings settings, User& user) {
    int choice;
    string MenuParagraph[3]{ "      Вручну       ", "    Автоматично    ", "       Вихід       " };
    choice = startMenu(X_MENU * 2, Y_ALL, 11, 3, MenuParagraph, "Розстановка", settings);
    Field userField, botField;
    if (choice == 0) userFieldPlacing(userField, settings);
    else if (choice == 1) autoFieldPlacing(userField);
    else goto end;
    autoFieldPlacing(botField);
    play(userField, botField, settings, user);
    saveUser(user);
end:
    SetColor(BLACK, BLACK);
}

void botPlay(Settings& settings) {
    Field botField1, botField2;
    autoFieldPlacing(botField1);
    autoFieldPlacing(botField2);
    botPlay(botField1, botField2, settings);
}

void resumeGame(Settings settings, User user) {
    if (!gameExists(user)) return;
    Field Field1, Field2; 
    loadUserGame(user, Field1, Field2);
    play(Field1, Field2, settings, user);
}

bool changeDifficulty(User user, Settings settings) {
    string menuParagraph[2] = {"Easy", "Hard"};
    return startMenu(X_MENU * 2, Y_ALL, 9, 2, menuParagraph, "Складність", settings);
}

void Statistics(User user, Settings settings) {
    if (user.games == 0) {
        user.percentOfWin = 0;
    }
    else {
        user.percentOfWin = (user.wins * 100 / user.games);
    }
    saveUser(user);
    StatisticMenu(X_MENU * 2, Y_ALL, 13, user, settings);
}

int changeColorSettings(Settings settings) {
    return changeColor(settings);
}

void Exit(bool& active) {
    SetColor(WHITE, BLACK);
    system("cls");
    SetCursorPosition(X_MENU * 2.3, Y_ALL);
    cout << "Goodbye!";
    SetColor(BLACK, BLACK);
    exit(0);
}

int main()
{
    unsigned int choice;
    Settings defaultSettings{ '*', false, RED_FADE };
    Settings settings;
    srand(time(NULL));
    Cyrrilic();
    ConsoleCursorVisible(false);

    User user;
    string accountMenuParagraph[2] = { "      Логін      ", "    Реєстрація   " };
    login:
    choice = startMenu(X_MENU * 2, Y_ALL, 10, 2, accountMenuParagraph, "Система акаунтів", defaultSettings);
    switch (choice) {
    case 0: user = loginUser(); break;
    case 1: user = registerNewUser(); break;
    default: break;
    }
    string SettingsMenuParagraph[6]{ "    Демонстрація   ", "     Колір меню    ", "     Складність    ", "    Символ меню    ", "      Скинути      ", "       Вихід       " };
    string MainMenuParagraph[6]{ "       Грати       ", "   Продовжити гру  ", "    Налаштування   ", "     Статистика    ", "  Змінити аккаунт  ", "       Вихід       " };
    bool active = true;
    while (active) {
        settings.graphUnit = user.graphUnit;
        settings.difficulty = user.difficulty;
        settings.color = (ConsoleColor)user.color;
        SetColor(settings.color, BLACK);
        choice = startMenu(X_MENU * 2, Y_ALL, 11, 6, MainMenuParagraph, user.login, settings);
        switch (choice) {
        case 0: startGame(settings, user); break;
        case 1:
            resumeGame(settings, user);
            break;
        case 2: choice = startMenu(X_MENU * 2, Y_ALL, 11, 6, SettingsMenuParagraph, "Налаштування", settings); 
            switch (choice) {
            case 0:
                botPlay(settings);
                break;
            case 1:
                user.color = changeColor(settings);
                saveUser(user);
                break;
            case 2:
                user.difficulty = changeDifficulty(user, settings);
                saveUser(user);
                break;
            case 3:
                user.graphUnit = changeChar(settings);
                saveUser(user);
                break;
            case 4: settingsToDefault(user);
            default: break;
            }
            break;
        case 3: Statistics(user, settings);
            break;
        case 4: goto login;
            break;
        case 5: 
            Exit(active);
            break;
        default: break;
        }
    }
    
}
