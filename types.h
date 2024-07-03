#pragma once

typedef struct gameState {

	int start;

} GameState;

typedef struct timeb TimeB;

typedef struct COLOR
{
	int R;
	int G;
	int B;
	int A;
} Color;

typedef struct body {
	int x;
	int y;
} Body;

typedef struct Snake {
	Body* body;
	int length;
	int size;
	int direction;
} Snake;

typedef struct Apple {
	int x;
	int y;
	int size;
} Apple;