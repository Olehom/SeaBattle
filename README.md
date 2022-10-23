# Курсовий проект "Морський бій". 
#### Виконав Гомбош Олег. Навчальний заклад КА "ШАГ". Група: ПВ123
# Розділи
* ### [Вступ](#Вступ)
* ### [Опис основних завдань](#Опис основних завдань)

# Вступ
Мета і завдання: 
* Визначити та описати завдання які мають бути вирішені.
* Створити гру [Морський бій](https://uk.wikipedia.org/wiki/%D0%9C%D0%BE%D1%80%D1%81%D1%8C%D0%BA%D0%B8%D0%B9_%D0%B1%D1%96%D0%B9_(%D0%BD%D0%B0%D1%81%D1%82%D1%96%D0%BB%D1%8C%D0%BD%D0%B0_%D0%B3%D1%80%D0%B0)). 
* Описати процес розробки. 
* Протестувати гру та виправити виявлені помилки. 
* Зробити висновки.

При розробці гри, потрібно використати **C++** та **тільки** вбудовані бібліотеки. Також, вказати **джерела** які були 
використані в процесі розробки.

# Опис основних завдань
1. Створити структуру ігрового поля та додати можливість взаємодіяти з нею відповідно до правил гри (Розставляти різні виді кораблів, руйнувати їх та інше).
2. Створити зрозумілий графічний інтерфейс та додати можливість модифікувати деякі його елементи.
3. Створити систему акаунтів та дозволити кожному користувачу змінювати налаштування UI під себе.
4. Реалізувати можливість зберігати й продовжувати гру.
5. Розробити алгоритм ведення бою для штучного інтеллекту.

# Основні моменти розробки
#### Реалізація ігрового поля та основних правил гри
Створити ігрове поле я вирішив з допомогою структури "Field"
```
struct Field {
Cell cells[10][10];                     
int ships[10]{ 4,3,3,2,2,2,1,1,1,1 };
int shipCount{ 0 };
};
```

Cell cells[10][10] - Це масив клітинок 10х10. Клітинку я вирішив створити з допомогою структури Cell. 
Це зумовлено тим, що елементи поля я записую як цілі числа. Але при ручному розміщенні, користувач при переміщенні, 
перезаписує їх під інше число (в данному випадку, число корабля, який він розміщує). Тому було створено структуру "Cell", 
в якій знаходяться 2 змінні, перша відповідає за змінні клітинки, друга за незмінні (підтвердежні) 

```
struct Cell {
    int type = 0;         // Змінна клітинка
    int verifyType = 0;   // Незмінна клітинка
};
```
int ships[10]{ 4,3,3,2,2,2,1,1,1,1 } - Розмір масиву це кількість кораблів. Число, це кількість палуб корабля.

int shipCount{ 0 }; - Це змінна, яка дозволяє розставити кораблі так, щоб у кожного був свій номер у масиві. 
Це в свою чергу, дозволяє визначити індекс корабля в масиві "ships" вже після розстановки і виявити, чи він потоплений.

Для того щоб не заплутатись яка цифра за який елемент відповідає, я створив enum "Cells":
```
enum Cells {
    EMPTY = 0, SHIP_PLACE = 1, AIM = 2, 
    DESTROY_SHIP = 1, MISS = 2, AROUND = 3,
    AROUND_DESTROY = 4, SHIP = 5,
};
```

І ось після того, як ми створили структуру поля, визначили за що відповідає кожна клітинка, настав час створити функцію,
яка буде виводити поле так, як потрібно нам.
```
void fieldPrint(Field& Field, const int xMove, const int yMove, int y, bool isBot, int x, ConsoleColor color) {
    string numeric = "ABCDEFGHIJ";
    int i, j;
    cout << endl;
    for (i = 0; i < MAP_SIZE; i++) {
        cout << "  ";
        for (j = 0; j < MAP_SIZE; j++) {
            if (Field.cells[i][j].type == AIM) {
                SetColor(BLACK, RED_FADE);
                SetCursorPosition((x + j) * 2, y + i);
                cout << "><";
                continue;
            }
            else if (Field.cells[i][j].verifyType == DESTROY_SHIP) {
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
            else if (Field.cells[i][j].type == EMPTY) {
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
```
Функція приймає такі аргументи: Поле, координати х і у це знаходиться користувач при розстановці, чи це бот, де розміщати поле
в консолі та колір розмітки. Працює вона ніби "накладанням". Спочатку перевіряються ті клітинки, яки мають більший приорітет,
потім ті що менший. Умовно це можна уявити собі так "Прицел > Зламана палуба > Промах > Корабель при розставленні > 
Обводка зруйнованого корабля > Обводка звичайного корабля > Корабель > Пустота". Кожна клітинка має свій колір, що дозволяє
легше сприймати гру для звичайного користувача. В результаті ми отримуємо таке поле:

![field.png](images/field.png)

Для зручності, я додатково створив enum "Positions", який визначає типові координати для розміщення певних елементів:
```
enum Positions {
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
```
Думаю по назвам зрозуміло що за що відповідає.
### Розміщення

Користувачеві доступні два варіанта розміщення: Автоматично та вручну.
###### Автоматично
Автоматичне розміщення відбувається з допомогою цієї функції:

```
void autoFieldPlacing(Field& Field) {
    for (int i = 0; i < 10; i++) {
        autoShipPlacing(Field, Field.ships[i]);
    }
}
```
В неї ми передаємо посилання на поле та 10 раз викликаємо наступну функцію:
```
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
```
Вона в свою чергу теж приймає посилання на поле і також розмір корабля, який потрібно розмістити.
Потім запускається цикл, який виконується до того моменту, поки корабель не буде поставлено.
В тілі циклу обираються рандомні координати та напрямок корабля. Якщо він не влізає, тоді цикл починається спочатку
з допомогою "goto start". Чому не було використано звичайний "continue"? Тому-що в циклі "while" ще існує вкладенний цикл
"for" і коли ми пишемо "continue", він починає спочатку вкладений цикл, що ніяк не вирішує проблеми.

###### Вручну
Ручне розміщення відбувається з допомогою цієї функції:

```
void userFieldPlacing(Field& Field, Settings settings) {
    for (int i = 0; i < 10; i++) {
        shipPlace(Field, Field.ships[i], settings);
    }
}
```
В неї ми передаємо посилання на поле і налаштування. Налаштування потрібні щоб розмітка мала такий колір, який задав користувач. 
Потім 10 раз викликаємо наступну функцію:
```
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
```
Думаю пояснювати що й чому приймає ця функція не має сенсу, але поясню те, як вона працює.
Спочатку ми малюємо поле з допомогою функції "fieldPrint()", потім користувач натискає будь-яку клавішу. При натисканні cтрілок, 
ми прибираємо корабель, визначаємо нові координати та вже на нових координатах, записуємо корабель як звичайний "cell.type". 
Якщо користувач натискає "Enter", тим самим даючи поняти що він визначився з місцем розташування корабля, то ми перевіряємо чи не є поряд з цим
кораблем ще одного корабля, записуємо цей корабель вже у "cell.verifyType" і припиняємо виконання циклу.

Ось який це має вигляд:

![placing.gif](images/placing.gif)

#### Руйнування кораблів

Руйнування кораблів поділяється на два типа. Руйнування користувачем та ботом. 

###### Руйнування користувачем

У функції руйнування корабля користувачем, ми передаємо поле бота, дію яка буде відображена після вистрілу, налаштування 
для кольору розмітки, поле користувача і акаунт користувача щоб в разі чого, при натисканні ESC гра зберігалась й припинялась.
```
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
```
Ця функція повертає те, чи влучив користувач. У функції цикл працює до того моменту, поки не буде повернено якесь значення.
Код дуже схожий до розставлення корабля, але відрізняється тим, що тут замість корабля, ми переміщуємо приціл і при натисканні
відбувається вистріл, який вже визначає, чи потрапила людина. Якщо так, то від корабля віднімається одна палуба, сама палуба стає зруйнованою,
перевірається чи взагалі є ще живі палуби у корабля, якщо ні, тоді він обводиться і на екран виведеться повідомлення про те, 
що корабель зруйновано, або те, що зруйновано одну палубу, якщо ще є живі палуби. В разі якщо людина промахнулась, тоді клітинка
позначається як "промах" і по ній більше неможливо вистрілити, також виводиться повідомлення про промах.

Ось який це має вигляд:</br>![player_destroying.gif](images/player_destroying.gif)

### Реалізація графічного інтерфейсу

В проекті реалізовано більше 5 різних меню (Меню логіну, меню налаштувань, головне меню, тощо.) Тому я вирішив написати для
них одну функцію.
```
int startMenu(const int x, const int y, int menuSize, int arrSize, string* MenuParagraph, string Name, Settings settings) {
    system("cls");
    int key, activeOption{ 0 };
    MenuPrint(menuSize, x, y, settings.graphUnit, Name, settings.color);        // Виведення меню без кнопок
    MenuOptionsPrint(menuSize, x, y, activeOption, MenuParagraph, arrSize, settings.color);     // Виведення кнопок
    while (true) {
        key = _getch();
        if (key != -1) {
            switch (key) {
            case ENTER:
                system("cls");
                return activeOption;
                break;
            case UP_ARROW:
                if (activeOption > 0) activeOption--;
                else activeOption = arrSize - 1;
                MenuOptionsPrint(menuSize, x, y, activeOption, MenuParagraph, arrSize, settings.color);
                break;
            case DOWN_ARROW:
                if (activeOption < arrSize - 1) activeOption++;
                else activeOption = 0;
                MenuOptionsPrint(menuSize, x, y, activeOption, MenuParagraph, arrSize, settings.color);
                break;
            default: break;
            }
        }
    }
}
```
Вона приймає x та y розміщення, розмір меню, розмір массиву з кількістю пунктів, назву меню та налаштування. Працює поки
не буде повернено якесь зі значень. В цілому код досить простий і зрозумілий. Якщо людина намагається вибратись за межі
масиву, тоді вона або повертається на початок, або переходить у кінець.

Ось як це виглядає:</br>![menuDemo.gif](images/menuDemo.gif)

Також UI може бути модифікований користувачем. Можна змінити колір та символ. Це відбувається у цих двох функціях
```
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
```
```
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
if (choice < 1 || choice > 16) goto begin;
return choice;
}
```
Людина обирає бажанний параметр і змінює його так, як вона того захоче. Функції написано досить зрозуміло, тому нема сенсу
їх додатково пояснювати, краще продемонструю як вони працюють:</br>![settingsChange.gif](images/settingsChange.gif)
