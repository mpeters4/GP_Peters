//#include "Game.h"
#include "TextureLoader.h"
#include <iostream>
#include <ctime>


SDL_Texture* playerTexture;
Object player;
SDL_Rect desR, srcR;
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
	
	player.setDest(50, 50, 128, 300);
	player.setSrc(0, 0, 64, 64);
	player.setImg("model/player.png", renderer);

	playerTexture = TextureLoader::LoadTexture("model/player.png", renderer);
}

void Game::eventHandler() {
	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type){
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (e.key.keysym.sym == SDLK_LEFT) {
			std::cout << "LEFT down" << "\n" ;
			desR.x --;
		}
		else if (e.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT down" << "\n";
			desR.x ++;
		}
		if (e.key.keysym.sym == SDLK_SPACE) {
			std::cout << "SPACE down" << "\n";
			/*double elapsed_secs ; 
			clock_t begin = clock();
			if(e.key.keysym.sym == SDLK_SPACE)
			clock_t end = clock();
			elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;*/
		}
		break;
	case SDL_KEYUP:
		if (e.key.keysym.sym == SDLK_LEFT) {
			std::cout << "LEFT up" << "\n";
		}
		else if (e.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT up" << "\n";
		}
		if (e.key.keysym.sym == SDLK_SPACE) {
			std::cout << "SPACE up" << "\n";
		}
		break;
	default:
		break;
	}

}

void Game::draw(Object o) {
	SDL_Rect dest = o.getDest();
	SDL_Rect src = o.getSrc();
	//SDL_RenderCopyEx(renderer, o.getTex(), &src, &dest, 0,NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, o.getTex(), &src, &dest);
	
}

void Game::update() {
	cnt++;
	//std::cout << move<<"\n";

	
	desR.w = 64;
	desR.h = 64;
	//move = cnt;
	//destR.x = move;
	//destR.y = move;
	//std::cout << "Counter: " << cnt << std::endl;
}

void Game::render() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, playerTexture, NULL, &desR);
	//SDL_RenderPresent(renderer);
	draw(player);

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}