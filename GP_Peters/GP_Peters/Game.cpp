#include "Game.h"
#include <iostream>


SDL_Texture* playerTexture;
SDL_Rect destR, srcR;

Game::Game() {}
Game::~Game() {

}

void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen) {
	int fs = 0;
	if (fullscreen) {
		fs = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, x, y, width, height, fs);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 100, 150, 100, 255);
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}
	SDL_Surface* tmpSurface = IMG_Load("model/player.png");
	playerTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
}

void Game::eventHandler() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type){
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	} 
}

void Game::update() {
	cnt++;
	destR.w = 64;
	destR.h = 64;
	destR.x = cnt;
	destR.y = cnt;
	std::cout << "Counter: " << cnt << std::endl;
}

void Game::render() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, playerTexture, NULL, &destR);
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}