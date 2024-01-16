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
#include "SDL_ttf.h"


Map* map;
Manager manager;
Enemy enemyClass;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;
bool Game::gameOver = false;
int enemyKillCount = 0;

auto& player(manager.addEntity());

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

		if (TTF_Init() == -1) {
			std::cerr << "SDL_ttf initialization error: " << TTF_GetError() << std::endl;
		}

		isRunning = true;
	}

	// SDL_ttf
	font = TTF_OpenFont("src/Assets/BrownieStencil.ttf", 34);
	if (!font)
	{
		std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
	}

	textColor = { 255, 255, 255, 255 }; // White color

	assets->AddTexture("terrain", "src/Assets/MapAssets/map_assets.png");
	assets->AddTexture("player", "src/Assets/player_anims.png");
	assets->AddTexture("projectile", "src/Assets/Projectile.png");
	assets->AddTexture("playerProjectile", "src/Assets/projectile_animations.png");
	assets->AddTexture("enemy", "src/Assets/enemy_animations.png");

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
	enemyClass.spawnEnemy(map, manager, player);

	for (auto& e : manager.getGroup(Game::groupEnemies))
	{
		Vector2D playerPos = player.getComponent<TransformComponent>().position;
		Vector2D enemyPos = e->getComponent<TransformComponent>().position;

		e->direction = playerPos - enemyPos;
		e->direction.normalize();

		float speed = 2.0f; // Adjust as needed
		e->getComponent<TransformComponent>().position += e->direction * speed;
	}

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
		if (p->getComponent<SpriteComponent>().identifier == "playerProjectile")
		{
			for (auto& e : enemies)
			{
				if (Collision::AABB(e->getComponent<ColliderComponent>().colliderRect, p->getComponent<ColliderComponent>().colliderRect))
				{
					p->destroy();
					e->destroy();
					enemyKillCount++;
					enemyClass.reduceSpawnedCounter();
				}
			}
		}
	}

	for (auto& e : enemies)
	{
		if (Collision::AABB(e->getComponent<ColliderComponent>().colliderRect, player.getComponent<ColliderComponent>().colliderRect))
		{
			gameOver = true;
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

	/* Render enemy kill counter */
	std::string counterText = "Enemies Killed: " + std::to_string(enemyKillCount);

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, counterText.c_str(), textColor);
	if (!textSurface) {
		std::cerr << "Error creating text surface: " << TTF_GetError() << std::endl;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h }; // Adjust the position

	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	delete assets;
	delete map;
	TTF_CloseFont(font);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();

	SDL_Quit();
	Log("Game Cleaned!");
}

void Game::renderGameOver()
{
	std::string gameOverText = "Game Over!" ;
	font = TTF_OpenFont("src/Assets/Bloody.otf", 200);
	textColor = { 255, 0, 0, 255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
	if (!textSurface) {
		std::cerr << "Error creating text surface: " << TTF_GetError() << std::endl;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect = { WINDOW_WIDTH / 2 - 450, WINDOW_HEIGHT / 2 - 50, textSurface->w, textSurface->h}; // Adjust the position

	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	SDL_RenderPresent(renderer);
}