#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys\timeb.h> 
#include <stdlib.h>
#include <SDL.h>
#include "types.h"
#include "constants.h"
#include "server.h"
#include "game.h"

const int H = 800;
const int W = 1000;
const int SPEED = 50;
const int BLOCK_SIZE = 20;
const int SNAKE_START_LENGTH = 10;


int main() {

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	GameState state = { 0 };


	bool isRunning = true;
	bool isGameRunning = true;
	SDL_Event event;
	TimeB lastUpdate;

	Apple apple = {0};
	Snake* snakes = (Snake*)malloc(sizeof(Snake) * 2);

	createApple(&apple);

	for (int i = 0; i < 2; i++)
	{
		if (createSnake(snakes + i, i) > 0) return 1;
	}

	//startServer(&state);


	ftime(&lastUpdate);


	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				isRunning = false;
			}

		}

		if (isGameRunning) {
			//printf("running");

			int updateResult = update(snakes, &apple, &lastUpdate, 1);
			if (updateResult == 1) return 1;
			render(renderer, snakes, &apple);

			if (snakeCollitionDetection(&snakes[0]) > 0) {
				renderGameOver(renderer);
				isGameRunning = false;
			}
		}

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}