#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "Log.h"

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

	static SDL_Renderer* renderer;

private:
	int counter = 0;
	bool isRunning;
	SDL_Window* window;

};