#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int x, int y, int width, int height, bool fullscreen);
	
	void eventHandler();
	void update();
	void render();
	void clean();
	void loadMap();
	void drawMap();
	bool running() { return isRunning; }

private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
	int cnt = 0;
};