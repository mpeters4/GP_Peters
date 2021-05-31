#include "Game.h"
#include "TextureLoader.h"
#include <iostream>
#include <ctime>

using namespace std;

#define TILE_SIZE 32


Object player;
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
			player.dest.x--;

		}
		else if (e.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT down" << "\n";
			player.dest.x++;

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
				Object tmp;
				tmp.setImg("model/tiles.png", renderer);
				tmp.setSrc(((current - 1)*TILE_SIZE), 0, TILE_SIZE, TILE_SIZE);
				tmp.setDest((j*TILE_SIZE )+ x, (i*TILE_SIZE) + y, TILE_SIZE, TILE_SIZE);
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



void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

}