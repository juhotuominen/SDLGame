#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "Log.h"
#include <vector>

class ColliderComponent;

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

	static void AddTile(int srcX, int srcY, int xpos, int ypos);
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static bool isRunning;

private:
	int counter = 0;
	
	SDL_Window* window;

};