#include "Game.h"

Game* game = nullptr;
int main(int argc, char *argv[]) {
	game = new Game();
	//Initialisieren des Fensters
	game->init("Climb it", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (game->running()) {
		game->eventHandler();
		game->update();
		game->render();
	}
	game->clean();
	return 0;
}