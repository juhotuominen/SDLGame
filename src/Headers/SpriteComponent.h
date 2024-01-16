#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include "AssetManager.h"
#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;
	std::string identifier;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(std::string id)
	{
		setTexture(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		
		animated = isAnimated;
		identifier = id;

		Animation idle = Animation(0, 3, 200);
		Animation walk = Animation(1, 8, 100);
		Animation down = Animation(2, 8, 200);
		Animation up = Animation(3, 3, 300);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);
		animations.emplace("Down", down);
		animations.emplace("Up", up);

		play("Idle");
		setTexture(id);
	}

	~SpriteComponent()
	{}

	void setTexture(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

	}

	void update() override
	{

		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

	
};