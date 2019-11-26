#pragma once
#include "AnimationData.h"
#include "JSONParser.h"
#include "graphics/sprite.h"

class SpriteBasedAnimation
{
public:
	SpriteBasedAnimation();
	~SpriteBasedAnimation();
	void init(const char* tex, const char* ske, const char* image, gef::Platform& plat);

	void update(float dt, gef::Vector2 screenPos);

	gef::Sprite* getSprite() { return sprite; }


private:

	void spriteSetup(SubTexture* sub, gef::Vector2 screenPos);
	SpriteJSONData* json_data_;

	float current_frame;

	gef::Sprite* sprite;
	gef::Texture* sprite_texture_;

	float time;

};

