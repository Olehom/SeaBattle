#include <iostream>
#include <Windows.h>
#include <time.h>
#include <string>
#include "Header.h"
#define MAP_SIZE 10

using namespace std;

void AIDestroy(Field& Field, AI& Ai) {
	Ai.decksCount--;
	Field.ships[Field.cells[Ai.y][Ai.x].verifyType - SHIP]--;
	Field.cells[Ai.y][Ai.x].verifyType = DESTROY_SHIP;

}

bool AIDefinition(Field& Field, AI& Ai, int dir) {
	if (Field.cells[Ai.y][Ai.x].verifyType == MISS || Field.cells[Ai.y][Ai.x].verifyType == DESTROY_SHIP || Field.cells[Ai.y][Ai.x].verifyType == AROUND_DESTROY) {
		Ai.dir = dir;
		Ai.x = Ai.hintX;
		Ai.y = Ai.hintY;
		return true;
	}

	if (botHint(Field, Ai.y, Ai.x)) {
		AIDestroy(Field, Ai);
		return true;
	}
	else {
		Ai.dir = dir;
		Ai.x = Ai.hintX;
		Ai.y = Ai.hintY;
		return false;
	}
}

bool AIHint(Field& Field, AI& Ai) {
	switch (Ai.dir) {
	case DOWN: 
		if (Ai.y + 1 > MAP_SIZE){
			Ai.dir = 2;
			Ai.x = Ai.hintX;
			Ai.y = Ai.hintY;
			return false;
		}
		Ai.y++;
		return AIDefinition(Field, Ai, UP);
	case LEFT:
		if (Ai.x - 1 < 0) {
			Ai.dir = 3;
			Ai.x = Ai.hintX;
			Ai.y = Ai.hintY;
			return false;
		}
		Ai.x--;
		return AIDefinition(Field, Ai, RIGHT);
	case UP:
		if (Ai.y - 1 < 0) {
			Ai.dir = 1;
			Ai.x = Ai.hintX;
			Ai.y = Ai.hintY;
			return false;
		}
		Ai.y--;
		return AIDefinition(Field, Ai, LEFT);
	case RIGHT:
		if (Ai.x + 1 > MAP_SIZE) {
			Ai.dir = 0;
			Ai.x = Ai.hintX;
			Ai.y = Ai.hintY;
			return false;
		}
		Ai.x++;
		return AIDefinition(Field, Ai, DOWN);
	default: break;
	}
}

bool AIAttack(Field& Field, AI& Ai, int& action) {
	if (Ai.decksCount < 1) {
		while (true) {
			Ai.hintX = rand() % MAP_SIZE;
			Ai.hintY = rand() % MAP_SIZE;
			if (Field.cells[Ai.hintY][Ai.hintX].verifyType == MISS || Field.cells[Ai.hintY][Ai.hintX].verifyType == DESTROY_SHIP || Field.cells[Ai.hintY][Ai.hintX].verifyType == AROUND_DESTROY) continue;
			if (botHint(Field, Ai.hintY, Ai.hintX)) {
				Ai.x = Ai.hintX;
				Ai.y = Ai.hintY;
				Ai.decksCount = Field.ships[Field.cells[Ai.y][Ai.x].verifyType - SHIP];
				Ai.decksCount--;
				Field.ships[Field.cells[Ai.y][Ai.x].verifyType - SHIP]--;
				Field.cells[Ai.y][Ai.x].verifyType = DESTROY_SHIP;
				return true;
			}
			action = 1;
			Ai.hintX = -1;
			Ai.hintY = -1;
			return false;
		}
	}
	else {
		return (AIHint(Field, Ai));
	}
}
