//#include "Game.h"
#include "TextureLoader.h"
#include <iostream>


SDL_Texture* playerTexture;
SDL_Rect destR, srcR;
int move = 0;

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

	playerTexture = TextureLoader::LoadTexture("model/player.png", renderer);

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
	std::cout << move<<"\n";

	
	destR.w = 64;
	destR.h = 64;
	move = cnt;
	destR.x = move;
	destR.y = move;
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