void shipPlace(Field& Field, int shipSize) {
    int i, dx1, dx2, dy1, dy2, key, xMove{ 0 }, yMove{ 0 };
    bool vertical{ true }, insertedPossible{ true }, isWork{ true };
start: while (isWork) {
    insertedPossible = true;
    dy1 = 0; dx1 = 0; dx2 = 0; dy2 = 0;
    fieldPrint(Field, xMove, yMove, Y_ALL, false, X_PLAYER_PLACE);
    key = _getch();
    switch (key) {
    case ENTER:                 // ѕ≥дтвердженн€ типу
        switch (vertical) {
        case true: dy1 = 1; break;
        case false: dx1 = 1; break;
        }
        // if(vertical) { if(Field.ceils[y][x] == AROUND) x++ else y++ }
        if (insertedPossible) {     // якщо неможливо вставити корабель, то повернутис€ назад.
            for (i = 0; i < shipSize; i++) {
                if (Field.ceils[yMove + dy1 * i][xMove + dx1 * i].verifyType == AROUND || Field.ceils[yMove + dy1 * i][xMove + dx1 * i].verifyType >= SHIP) insertedPossible = false;
                if (!insertedPossible) goto start;
            }
        }
        else goto start;

        for (i = 0; i < shipSize; i++) {    // ¬ставленн€ корабл€
            setType(Field, yMove + dy1 * i, xMove + dx1 * i, EMPTY);
            setVerifyType(Field, yMove + dy1 * i, xMove + dx1 * i, SHIP + Field.shipCount);
        }
        // ≤нкремент щоб у кожного корабл€ був св≥й ≥ндекс
        Field.shipCount++;
        fieldAroundInsert(Field, xMove, yMove, AROUND);
        isWork = false;
        break;
    case UP_ARROW:          // «двиг верх
        if (yMove < 1) break;   // якщо не вписуЇтьс€ в розм≥р пол€, тод≥ перестаЇ виконуватись
        switch (vertical) {
        case true: dy1 = 1; dy2 = 1; break;
        case false: dx1 = 1; dx2 = 1; break;
        }
        for (i = 0; i < shipSize; i++) {    // «м≥на положенн€ корабл€
            setType(Field, yMove + dy1 * i, xMove + dx1 * i, EMPTY);
            setType(Field, yMove + dy2 * i - 1, xMove + dx2 * i, SHIP_PLACE);
        }
        yMove--;
        break;

    case DOWN_ARROW:        // «двиг вниз
        if (vertical && yMove + shipSize + 1 > MAP_SIZE || !vertical && yMove + 1 > MAP_SIZE - 1) break;   // якщо не вписуЇтьс€ в розм≥р пол€, тод≥ перестаЇ виконуватись

        switch (vertical) {
        case true: dy1 = -1; dy2 = 1; break;
        case false: dx1 = 1; dx2 = 1; break;
        }

        for (i = 0; i < shipSize; i++) {    // «м≥на положенн€ корабл€
            setType(Field, yMove + dy1 * i, xMove + dx1 * i, EMPTY);
            setType(Field, yMove + dy2 * i + 1, xMove + dx2 * i, SHIP_PLACE);
        }
        yMove++;
        break;

    case LEFT_ARROW:        // «двиг вл≥во
        if (xMove < 1) break;   // якщо не вписуЇтьс€ в розм≥р пол€, тод≥ перестаЇ виконуватись

        switch (vertical) {
        case true: dy1 = 1; dy2 = 1; break;
        case false: dx1 = 1; dx2 = 1;; break;
        }
        for (i = 0; i < shipSize; i++) {    // «м≥на положенн€ корабл€
            setType(Field, yMove + dy1 * i, xMove + dx1 * i, EMPTY);
            setType(Field, yMove + dy2 * i, xMove + dx2 * i - 1, SHIP_PLACE);
        }
        xMove--;
        break;

    case RIGHT_ARROW:       // «двиг вправо
        if (!vertical && xMove > MAP_SIZE - shipSize - 1 || vertical && xMove + 1 > MAP_SIZE - 1) break; // якщо не вписуЇтьс€ в розм≥р пол€, тод≥ перестаЇ виконуватись

        switch (vertical) {
        case true: dy1 = 1; dy2 = 1; break;
        case false:
            dx1 = shipSize - 1; dx2 = shipSize;

            for (i = 0; i < shipSize; i++) {
                setType(Field, yMove, xMove + dx1 - i, EMPTY);
                setType(Field, yMove, xMove + dx2 - i, SHIP_PLACE);
            }
            xMove++;
            goto start;
            break;
        }
        for (i = 0; i < shipSize; i++) {    // «м≥на положенн€ корабл€
            setType(Field, yMove + dy1 * i, xMove + dx1 * i, EMPTY);
            setType(Field, yMove + dy2 * i, xMove + dx2 * i + 1, SHIP_PLACE);
        }
        xMove++;
        break;

    case SPACE:             // ѕоворот корабл€ в одне з чотирьох направлень
        if (vertical && xMove + shipSize < MAP_SIZE + 1) {
            dy1 = 1; dx2 = 1;
            vertical = !vertical;
        }
        else if (!vertical && yMove + shipSize < MAP_SIZE + 1) {
            dx1 = 1; dy2 = 1;
            vertical = !vertical;
        }
        for (i = 0; i < shipSize; i++) {    // ѕоворот корабл€
            setType(Field, yMove + dy1 * i, xMove + dx1 * i, EMPTY);
            setType(Field, yMove + dy2 * i, xMove + dx2 * i, SHIP_PLACE);
        }
    default: break;
    }
}
}

void userFieldPlacing(Field& Field) {
    int i;
    for (i = 0; i < 10; i++) {
        shipPlace(Field, Field.ships[i]);
    }
}