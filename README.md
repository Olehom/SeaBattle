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
