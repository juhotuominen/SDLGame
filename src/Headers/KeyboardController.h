#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	void update() override {

		transform->velocity.x = 0;
		transform->velocity.y = 0;
		sprite->play("Idle");
		sprite->spriteFlip = SDL_FLIP_NONE;

		if (keystates[SDL_SCANCODE_W]) 
		{
			transform->velocity.y = -1;
			sprite->play("Walk");
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
			sprite->play("Walk");
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
	}
};