#pragma once
#include <SDL.h>
#include "types.h"

void createApple(Apple* apple);
int createSnake(Snake* snake, int start);
int snakeCollitionDetection(Snake* snake);
int update(Snake* snakes, Apple* apple, TimeB* lastUpdateMs, int direction);
void render(SDL_Renderer* renderer, Snake* snakes, Apple* apple);
void renderGameOver(SDL_Renderer* renderer);