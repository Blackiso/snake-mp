#include <stdio.h>
#include <stdbool.h>
#include <sys\timeb.h> 
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "types.h"
#include "constants.h"


const Color BLACK = { 0, 0, 0, 255 };
const Color WHITE = { 255, 255, 255, 255 };
const Color GREEN = { 28, 230, 105, 255 };
const Color BLUE = { 73, 177, 242 };

void setColor(SDL_Renderer* renderer, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
}

void createApple(Apple* apple) {
	srand(time(NULL));
	apple->x = (rand() % (W / BLOCK_SIZE)) * BLOCK_SIZE;
	apple->y = (rand() % (H / BLOCK_SIZE)) * BLOCK_SIZE;
	apple->size = BLOCK_SIZE;
}

int createSnake(Snake* snake, int start) {
	Body* body = malloc(sizeof(Body) * SNAKE_START_LENGTH);

	if (body == NULL) return 1;

	start = start == 0 ? 0 : H - BLOCK_SIZE;
	snake->body = body;
	snake->length = SNAKE_START_LENGTH;
	snake->size = BLOCK_SIZE;
	snake->direction = 1;

	for (int i = 0; i < snake->length; i++)
	{
		body[i].x = i * snake->size;
		body[i].y = start;
	}

	return 0;
}

void createNewSnakeHead(Snake* snake) {
	int lastIndex = snake->length - 2 >= 0 ? snake->length - 2 : 0;
	int X = snake->body[lastIndex].x;
	int Y = snake->body[lastIndex].y;

	switch (snake->direction)
	{
	case 1:
		X += snake->size;
		if (X == W) X = 0;
		break;
	case -1:
		X -= snake->size;
		if (X + snake->size == 0) X = W;
		break;
	case 2:
		Y += snake->size;
		if (Y == H) Y = 0;
		break;
	case -2:
		Y -= snake->size;
		if (Y + snake->size == 0) Y = H;
		break;
	}

	Body head = { X, Y };
	snake->body[snake->length - 1] = head;
}

int growSnake(Snake* snake) {
	Body* newBody = realloc(snake->body, sizeof(Body) * snake->length);
	if (newBody == NULL) return 1;
	snake->body = newBody;
	createNewSnakeHead(snake);
	return 0;
}

int appleCollitionDetection(Snake* snake, Apple* apple) {
	Body head = snake->body[snake->length - 1];

	if (head.x == apple->x && head.y == apple->y) {
		createApple(apple);
		snake->length++;
		return growSnake(snake);
	}

	return 0;
}

int snakeCollitionDetection(Snake* snake) {
	Body head = snake->body[snake->length - 1];

	for (int i = 0; i < snake->length - 2; i++)
	{
		if (snake->body[i].x == head.x && snake->body[i].y == head.y) return 1;
	}

	return 0;
}

int update(Snake* snakes, Apple* apple, TimeB* lastUpdateMs, int direction) {
	TimeB now;
	ftime(&now);

	if (((int)(1000.0 * (now.time - lastUpdateMs->time) + (now.millitm - lastUpdateMs->millitm))) > SPEED) {

		ftime(lastUpdateMs);

		for (int i = 0; i < 2; i++)
		{
			if (direction != 0 && snakes[i].direction * -1 != direction) {
				snakes[i].direction = direction;
			}

			if (snakes[i].length > 1) {
				for (int x = 0; x < snakes[i].length; x++) {
					snakes[i].body[x] = snakes[i].body[x + 1];
				}
			}

			createNewSnakeHead(&snakes[i]);
			if (appleCollitionDetection(&snakes[i], apple) > 0) {
				return 1;
			}
			if (snakeCollitionDetection(&snakes[i]) > 0) {
				return 2;
			}
		}

	}

	return 0;
}

void render(SDL_Renderer* renderer, Snake* snakes, Apple* apple) {
	setColor(renderer, BLACK);
	SDL_RenderClear(renderer);
	

	for (int i = 0; i < 2; i++)
	{
		if (i == 0) {
			setColor(renderer, WHITE);
		}
		else {
			setColor(renderer, BLUE);
		}

		for (int x = 0; x < snakes[i].length; x++) {
			Body* b = snakes[i].body;
			SDL_Rect rect = { b[x].x, b[x].y, snakes[i].size, snakes[i].size };
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	setColor(renderer, GREEN);
	SDL_Rect appleRect = { apple->x, apple->y, apple->size, apple->size };
	SDL_RenderFillRect(renderer, &appleRect);
	SDL_RenderPresent(renderer);
}

void renderGameOver(SDL_Renderer* renderer) {

	int size = 50;
	setColor(renderer, WHITE);

	for (int y = 0; y < H; y = y + size)
	{
		for (int x = 0; x < W; x = x + size)
		{
			SDL_Rect rec = { x, y, size, size };
			SDL_RenderFillRect(renderer, &rec);
			SDL_RenderPresent(renderer);
		}
	}

	TTF_Font* font = TTF_OpenFont("fonts/Symtext.ttf", 100);

	if (font == 0) {
		printf("Error: %s\n", TTF_GetError());
	}

	SDL_Color black = { 0, 0, 0 };

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "GAME OVER", black);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	int xCenter = (W / 2) - (surfaceMessage->w / 2);
	int yCenter = (H / 2) - (surfaceMessage->h / 2) - 20;

	SDL_Rect messageRect = { xCenter, yCenter, surfaceMessage->w, surfaceMessage->h };
	SDL_RenderCopy(renderer, message, NULL, &messageRect);

	SDL_RenderPresent(renderer);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}