#pragma once
#include <string>
#include "maths/matrix44.h"
#include "maths/matrix33.h"
#include <vector>
#include <map>

struct SubTexture {
	float frameY;
	float y;
	float frameWidth;
	float frameX;
	float frameHeight;
	float width;
	float height;
	std::string name;
	float x;
	gef::Matrix33 subtextureTransform;
};

struct TextureAtlas
{
	float width;
	std::string imagePath;
	float height;
	std::string name;
	//change from string
	std::map<std::string, SubTexture> sub_textures;
	gef::Matrix33 baseTransform;
};



struct SpriteSkin {

	std::vector<std::string> display_name;
};


struct SkinSlotDisplay {
public:

	std::string name = "";

};

struct Display
{
	std::vector<std::string> display_name;
};


struct Skin
{
	std::string name = "";
	Display* display = nullptr;
};

struct Animation {
public:

	std::string name;
	float duration;
};

struct SpriteAnimation : Animation {
	
	std::vector<float> displayFrame;
};






struct Armature
{
public:
	std::string type;
	float frameRate;
	std::string name;

};

struct SpriteArmature : Armature {

	SpriteAnimation* animation = nullptr;
	Skin* skin = nullptr;
};


struct SpriteJSONData {

	SpriteArmature* armiture = nullptr;
	TextureAtlas* texture_atlas = nullptr;
};



