#pragma once
#include "AnimationData.h"
#include "JSONParser.h"
#include "graphics/sprite.h"

class SpriteBasedAnimation
{
public:
	SpriteBasedAnimation();
	~SpriteBasedAnimation();
	void init(const std::string tex, const std::string ske, const std::string image, gef::Platform& plat);

	void update(float dt, gef::Vector2 screenPos);

	gef::Sprite* getSprite() { return sprite; }

	void cleanUp();


private:

	void spriteSetup(SubTexture* sub, gef::Vector2 screenPos);
	SpriteJSONData* json_data_;

	float current_frame;

	gef::Sprite* sprite;
	gef::Texture* sprite_texture_;

	float time;

};

