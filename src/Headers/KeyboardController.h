#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	Vector2D projPosition;
	Game game;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		
		sprite = &entity->getComponent<SpriteComponent>();
	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	void update() override {

		transform->velocity.x = 0;
		transform->velocity.y = 0;

		if (transform->velocity.x == 0 && transform->velocity.y == 0)
		{
			sprite->play("Idle");
		}
		
		sprite->spriteFlip = SDL_FLIP_NONE;

		projPosition = transform->position;

		Uint32 currentTime = SDL_GetTicks();

		if (keystates[SDL_SCANCODE_W]) 
		{
			transform->velocity.y = -1;
			sprite->play("Up");
		}

		if (keystates[SDL_SCANCODE_A]) 
		{
			transform->velocity.x = -1;
			sprite->play("Walk");
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}

		if (keystates[SDL_SCANCODE_S]) 
		{
			transform->velocity.y = 1;
			sprite->play("Down");
		}

		if (keystates[SDL_SCANCODE_D]) 
		{
			transform->velocity.x = 1;
			sprite->play("Walk");
		}

		if (keystates[SDL_SCANCODE_ESCAPE])
		{
			Game::isRunning = false;
		}

		// Shooting logic
		if (keystates[SDL_SCANCODE_RIGHT] && (currentTime - lastShotTime >= shootingCooldown))
		{
			Game::assets->CreateProjectile(Vector2D(projPosition.x + 100, projPosition.y + 50), Vector2D(2, 0), 200, 2, "playerProjectile");
			lastShotTime = currentTime;
		}
		if (keystates[SDL_SCANCODE_LEFT] && (currentTime - lastShotTime >= shootingCooldown))
		{
			Game::assets->CreateProjectile(Vector2D(projPosition.x, projPosition.y + 50), Vector2D(-2, 0), 200, 2, "playerProjectile");
			lastShotTime = currentTime;
		}
		if (keystates[SDL_SCANCODE_UP] && (currentTime - lastShotTime >= shootingCooldown))
		{
			Game::assets->CreateProjectile(Vector2D(projPosition.x + 50, projPosition.y), Vector2D(0, -2), 200, 2, "playerProjectile");
			lastShotTime = currentTime;
		}
		if (keystates[SDL_SCANCODE_DOWN] && (currentTime - lastShotTime >= shootingCooldown))
		{
			Game::assets->CreateProjectile(Vector2D(projPosition.x + 50, projPosition.y + 100), Vector2D(0, 2), 200, 2, "playerProjectile");
			lastShotTime = currentTime;
		}
	}

private:
	Uint32 lastShotTime = 0;
	Uint32 shootingCooldown = 500;
};