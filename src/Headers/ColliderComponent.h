#pragma once

#include <string>
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect colliderRect;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		colliderRect.x = xpos;
		colliderRect.y = ypos;
		colliderRect.h = colliderRect.w = size;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("src/Assets/MapAssets/colTex.png");
		srcRect = { 0,0,32,32 };
		destRect = { colliderRect.x, colliderRect.y, colliderRect.w, colliderRect.h };

	}

	void update() override
	{
		if (tag != "terrain")
		{
			colliderRect.x = static_cast<int>(transform->position.x);
			colliderRect.y = static_cast<int>(transform->position.y);
			colliderRect.w = transform->width * transform->scale;
			colliderRect.h = transform->height * transform->scale;
		}

		destRect.x = colliderRect.x - Game::camera.x;
		destRect.y = colliderRect.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcRect, destRect, SDL_FLIP_NONE);
	}
};