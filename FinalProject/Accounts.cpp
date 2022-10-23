#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <string>
#include "Header.h"
#define MAP_SIZE 10

using namespace std;

void loadUserGame(const User& user, Field& Field1, Field& Field2) {
    string trash;
    int i, j;
    ifstream file(user.login + "Game" + ".txt");
    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            file >> Field1.cells[i][j].verifyType;
        }
        getline(file, trash);
    }
    getline(file, trash);
    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            file >> Field2.cells[i][j].verifyType;
        }
        getline(file, trash);
    }
    getline(file, trash);
    for (i = 0; i < MAP_SIZE; i++) {
        file >> Field1.ships[i];
    }
    getline(file, trash);
    for (i = 0; i < MAP_SIZE; i++) {
        file >> Field2.ships[i];
    }
    file.close();
}

void saveUserGame(const User& user, Field Field1, Field Field2) {
    int i, j;
    ofstream file(user.login + "Game" + ".txt");
    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            file << Field1.cells[i][j].verifyType << " ";
        }
        file << endl;
    }
    file << endl;
    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            file << Field2.cells[i][j].verifyType << " ";
        }
        file << endl;
    }
    file << endl;
    for (i = 0; i < MAP_SIZE; i++) {
        file << Field1.ships[i] << " ";
    }
    file << endl;
    for (i = 0; i < MAP_SIZE; i++) {
        file << Field2.ships[i] << " ";
    }
    file.close();
}

void saveUser(const User& user) {
    ofstream file(user.login + ".txt");
    file << user.login << endl;
    file << user.password << endl;
    file << user.color << endl;
    file << user.graphUnit << endl;
    file << user.difficulty << endl;
    file << user.wins << endl;
    file << user.games << endl;
    file << user.shipsDestroyer << endl;
    file << user.percentOfWin << endl;
    file.close();
}

void loadUser(User& user) {
    ifstream file(user.login + ".txt");
    getline(file, user.login);
    getline(file, user.password);
    file >> user.color;
    file >> user.graphUnit;
    file >> user.difficulty;
    file >> user.wins;
    file >> user.games;
    file >> user.shipsDestroyer;
    file >> user.percentOfWin;
    file.close();
}

bool gameExists(const User& user) {
    ifstream file(user.login + "Game" + ".txt");
    return file.good();
}

bool userExists(const User& user) {
    ifstream file(user.login + ".txt");
    return file.good();
}

User registerNewUser() {
    User user;

    user.color = RED_FADE;
    user.graphUnit = '*';
    user.difficulty = false;
    user.wins = 0;
    user.games = 0;
    user.shipsDestroyer = 0;
    user.percentOfWin = 0;
    string confirmPassword;
    while (true) {
        SetColor(BLACK, BLACK);
        system("cls");
        MenuPrint(15, X_MENU * 2, Y_ALL, '*', "Реєстрація", RED_FADE);
        SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 5);
        cout << "Логін: ";
        getline(cin, user.login);
        SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 6);
        cout << "Пароль: ";
        SetColor(BLACK, BLACK);
        getline(cin, user.password);
        SetColor(RED_FADE, BLACK);
        SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 7);
        cout << "Підтвердження: ";
        SetColor(BLACK, BLACK);
        getline(cin, confirmPassword);

        SetColor(RED, BLACK);
        bool valid = true;
        if (user.login.size() < 4) {
            SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 8);
            cout << "Логін менше 4 символів" << endl;
            valid = false;
        }
        if (user.password.size() < 5) {
            SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 9);
            cout << "Пароль менше 5 символів" << endl;
            valid = false;
        }
        if (user.password != confirmPassword) {
            SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 10);
            cout << "Паролі не однакові" << endl;
            valid = false;
        }
        if (userExists(user)) {
            SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 11);
            cout << "Користувач існує" << endl;
            valid = false;
        }
        SetColor(WHITE, BLACK);
        if (valid) {
            break;
        }
        else {
            system("pause");
        }
        system("cls");

    }
    saveUser(user);
    return user;
}

User loginUser() {

    User user;
    string password;
    while (true) {
        SetColor(BLACK, BLACK);
        system("cls");
        MenuPrint(15, X_MENU * 2, Y_ALL, '*', "Логін", RED_FADE);
        SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 5);
        cout << "Логін: ";
        getline(cin, user.login);
        SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 6);
        cout << "Пароль: ";
        SetColor(BLACK, BLACK);
        getline(cin, password);

        SetColor(RED, BLACK);
        bool valid = true;
        if (valid && !userExists(user)) {
            SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 7);
            cout << "Логіна не існує" << endl;
            valid = false;
        }
        else {
            loadUser(user);
        }

        if (valid && user.password != password) {
            SetCursorPosition(X_MENU * 2 + 3, Y_ALL + 8);
            cout << "Невірний пароль" << endl;
            valid = false;
        }

        SetColor(WHITE, BLACK);
        if (valid) {
            break;
        }
        else {
            system("pause");
        }
        system("cls");

    }
    return user;
}

void settingsToDefault(User& user) {
    user.color = RED_FADE;
    user.graphUnit = '*';
    user.difficulty = false;
}
