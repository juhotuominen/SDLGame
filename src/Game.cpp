#include "Headers/Game.h"
#include "Headers/TextureManager.h"
#include "Headers/Map.h"
#include "Headers/ECS.h"
#include "Headers/Components.h"
#include "Headers/Vector2D.h"
#include "Headers/Collision.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;

Map* map;
Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());


Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
		width = 1980;
		height = 1080;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		Log("Subsystems Initialized!...");

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			Log("*Window created!*");
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			Log("*Renderer created!*");
		}

		isRunning = true;
	}
	map = new Map();

	// ECS implementation

	Map::LoadMap("src/Assets/map16x16.map", 16, 16);

	player.addComponent<TransformComponent>(2);
	player.addComponent<SpriteComponent>("src/Assets/Player.png");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("src/Assets/Dirt.png");
	wall.addComponent<ColliderComponent>("wall");
}

void Game::handleEvents()
{
	

	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update() {

	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto cc : colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
		/*if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}*/
	}
	
}

void Game::render()
{
	SDL_RenderClear(renderer);
	manager.draw();
	SDL_RenderPresent(renderer);

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	Log("Game Cleaned!");
}

void Game::AddTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
}
