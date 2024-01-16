#pragma once
#include <random>
#include "SDL.h"
#include "Headers/Components.h"
#include "Map.h"
#include "Vector2D.h"

class Enemy
{
public:
	void spawnEnemy(Map* map, Manager& manager, Entity& player);
	int getRandomNumber(int min, int max);
	void reduceSpawnedCounter() { spawnedEnemies--;  }

private:
	int spawnedEnemies = 0;
};
