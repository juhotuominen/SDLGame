#include <iostream>
#include "Headers/Game.h"
#include "Headers/Constants.h"

Game* game = nullptr;

int main(int argc, char *argv[])
{

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();

	game->init("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false);

	while (game->running())
	{

		frameStart = SDL_GetTicks();

		game->handleEvents();

		if (!game->gameOver)
		{
			game->update();
			game->render();
		}
		else
		{
			game->update();
			game->renderGameOver();
		}

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}