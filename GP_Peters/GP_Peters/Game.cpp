#include "Game.h"
#include "TextureLoader.h"
#include <iostream>
#include <ctime>

using namespace std;

#define TILE_SIZE 32


Object player;
int move = 0;
bool jumpCharge = false;
Uint32 jumpTimer = 0;
const Uint8* keystate = SDL_GetKeyboardState(NULL);

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
			SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}
	
	player.setDest(800, 672, 64, 64);
	player.setSrc(0, 0, 64, 64);
	player.setImg("model/player.png", renderer);

	loadMap("res/1.map");
	
	

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
			player.move(-1, 1);

		}
		if (e.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT down" << "\n";
			player.move(1, 1);

		}
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_LEFT]) {
			printf("LEFT and SPACE Keys Pressed.\n");
			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			if ((SDL_GetTicks() - jumpTimer) >= 3000) {
				std::cout << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "jump!" << endl;

				jumpCharge = false;
				player.move(0, 32);
				player.move(-1, 32);

			}
			//player.move(-1, 32);
		}
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_RIGHT]) {
			printf("RIGHT and SPACE Keys Pressed.\n");
			//player.move(1, 32);
		}

		if (e.key.keysym.sym == SDLK_SPACE) {
			
			std::cout << "SPACE down" << "\n";

			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			if ((SDL_GetTicks() - jumpTimer) >= 3000) {
				std::cout << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "jump!" << endl;
				
				jumpCharge = false;
				player.move(0, TILE_SIZE * (SDL_GetTicks() - jumpTimer) * 0.001);
				
			}

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
			if (jumpCharge) {
				std::cout << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "Jumpheight: " << TILE_SIZE*(SDL_GetTicks() - jumpTimer) * 0.001 << endl;

				player.move(0, TILE_SIZE *(SDL_GetTicks() - jumpTimer) *0.001);
				jumpCharge = false;
			} 
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

	//std::cout << "Frame: "<< cnt << "\n";
	//move = cnt;
	//destR.x = move;
	//destR.y = move;
	//std::cout << "Counter: " << cnt << std::endl;
}

void Game::render() {
	SDL_RenderClear(renderer);
	drawMap();
	draw(player);
	SDL_RenderPresent(renderer);
	
}


void Game::loadMap(const char* filename) {
	int current, x, y, w, h;
	Object tmp;
	tmp.setImg("model/tiles.png", renderer);
	std::ifstream in(filename);
	if (!in.is_open()) {
		cerr << "ERROR wit loading file: " << filename << endl;
		return;
	}
	in >> w;
	in >> h;
	in >> x;
	in >> y;
	cout << w << endl << h << endl << x << endl << y << endl;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (in.eof()) {
				cerr << "Map reading ERROR" << endl;
				return;
			}
			in >> current;
			if (current != 0) {
				
				tmp.setSrc(((current - 1)*TILE_SIZE), 0, TILE_SIZE, TILE_SIZE);
				tmp.setDest((j*TILE_SIZE)+ x, (i*TILE_SIZE) + y, TILE_SIZE, TILE_SIZE);
				map.push_back(tmp);
			}
		}
	}
	in.close();
}

void Game::drawMap() {
	for (int i = 0; i < map.size(); i++) {
		draw(map[i]);
		//cout << "drawing tile on x: " << map[i].getDest().x << " y: " << map[i].getDest().y << endl;
	}
}

bool Game::checkCollision(Object a, Object b) {
	
}



void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

}