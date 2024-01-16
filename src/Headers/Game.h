#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Log.h"

#include <vector>
#include <cmath>
#include <algorithm> // for std::unique and std::distance
#include <ctime>
#include <cstdlib>

class Enemy;
class ColliderComponent;
class AssetManager;

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
	void renderGameOver();
	
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static bool gameOver;
	static SDL_Rect camera;
	static AssetManager* assets;

	TTF_Font* font;
	SDL_Color textColor;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupEnemies,
	};

private:
	
	SDL_Window* window;

};