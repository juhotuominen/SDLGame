#pragma once

#include <string>
#include "SDL.h"
#include "Components.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect colliderRect;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this);
	}

	void update() override
	{
		colliderRect.x = static_cast<int>(transform->position.x);
		colliderRect.y = static_cast<int>(transform->position.y);
		colliderRect.w = transform->width * transform->scale;
		colliderRect.h = transform->height * transform->scale;
	}

};