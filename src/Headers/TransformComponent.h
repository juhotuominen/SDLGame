#pragma once
#include "Constants.h"
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{

public:

	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 3;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int sc)
	{
		position.x = WINDOW_WIDTH / 2;
		position.y = WINDOW_HEIGHT / 2;
		scale = sc;
	}

	TransformComponent(int sc, float x, float y)
	{
		scale = sc;
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};