#include "Headers/Game.h"
#include "Headers/TextureManager.h"
#include "Headers/Map.h"
#include "Headers/ECS.h"
#include "Headers/Components.h"
#include "Headers/Vector2D.h"
#include "Headers/Collision.h"
#include"Headers/Constants.h"
#include "Headers/AssetManager.h"
#include "Headers/Enemy.h"
#include <random>


Map* map;
Manager manager;
Enemy enemyClass;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& enemy(manager.addEntity());

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

	assets->AddTexture("terrain", "src/Assets/MapAssets/map_assets.png");
	assets->AddTexture("player", "src/Assets/player_anims.png");
	assets->AddTexture("projectile", "src/Assets/Projectile.png");
	assets->AddTexture("playerProjectile", "src/Assets/playerProjectile.png");
	assets->AddTexture("enemy", "src/Assets/Enemy.png");

	map = new Map("terrain", 3, 32);

	// ECS implementation

	map->LoadMap("src/Assets/map.map", 48, 27);

	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));

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

void Game::update() 
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().colliderRect;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();
	enemyClass.spawnEnemy(map, manager);

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().colliderRect;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}
	
	for (auto& p : projectiles)
	{
		if (p->getComponent<SpriteComponent>().identifier != "playerProjectile")
		{
			if (Collision::AABB(player.getComponent<ColliderComponent>().colliderRect, p->getComponent<ColliderComponent>().colliderRect))
			{
				p->destroy();
			}
		}
		
		if (p->getComponent<SpriteComponent>().identifier == "playerProjectile")
		{
			for (auto& e : enemies)
			{
				if (Collision::AABB(e->getComponent<ColliderComponent>().colliderRect, p->getComponent<ColliderComponent>().colliderRect))
				{
					p->destroy();
					e->destroy();
					enemyClass.reduceSpawnedCounter();
				}
			}
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - (WINDOW_WIDTH / 2);
	camera.y = player.getComponent<TransformComponent>().position.y - (WINDOW_HEIGHT / 2);


	/* Camera boundaries.For some reason * 2 works with this resolution
	   bigger resolution results in white space after map ends.

	   TODO: Fix camera logic
	 */
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w * 2)
		camera.x = camera.w * 2;
	if (camera.y > camera.h * 2 )
		camera.y = camera.h * 2;

}

void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& c : colliders)
	{
		c->draw();
	}

	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& e : enemies)
	{
		e->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	SDL_RenderPresent(renderer);

}

void Game::clean()
{
	delete assets;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	Log("Game Cleaned!");
}
