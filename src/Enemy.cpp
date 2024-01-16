#include "Headers/Enemy.h"



void Enemy::spawnEnemy(Map* map, Manager& manager)
{
	static Uint32 lastEnemySpawn = 0;
	Uint32 currentTime = SDL_GetTicks();
	Uint32 spawnCooltime = 2000; // in milliseconds

	int randomX = getRandomNumber(200, 1300);
	int randomY = getRandomNumber(100, 800);
	auto& newEnemy(manager.addEntity());

	if (spawnedEnemies < 10 && (currentTime - lastEnemySpawn >= spawnCooltime))
	{
		newEnemy.addComponent<TransformComponent>(2, randomX * map->GetScale(), randomY * map->GetScale());
		newEnemy.addComponent<SpriteComponent>("enemy", false);
		newEnemy.addComponent<ColliderComponent>("enemy");
		newEnemy.addGroup(Game::groupEnemies);
		Log("EnemySpawned");

		spawnedEnemies++;

		lastEnemySpawn = currentTime;
	}
}

int Enemy::getRandomNumber(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}