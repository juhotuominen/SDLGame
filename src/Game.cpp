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

bool Game::isRunning = false;

Map* map;
Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

const char* mapfile = "src/Assets/MapAssets/map_assets.png";

enum groupLabels : std::size_t 
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders,
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

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

	Map::LoadMap("src/Assets/correctMap.map", 40, 20);

	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("src/Assets/player_anims.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
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

	Vector2D pVel = player.getComponent<TransformComponent>().velocity;
	int pSpeed = player.getComponent<TransformComponent>().speed;

	for (auto t : tiles)
	{
		t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed);
		t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed);
	}
	
}

void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}

	SDL_RenderPresent(renderer);

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	Log("Game Cleaned!");
}

void Game::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
	tile.addGroup(groupMap);
}
