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

struct Trans {
	float x = 0;
	float y = 0;
	float sk = 0;

};

struct SkinSlot {
	std::string name = "";
	std::string display_name = "";
	Trans transform;
	gef::Matrix33 transformMatrix;
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

struct BoneAnimation : Animation
{
	

	struct AnimationBoneData {
		struct rotateFrameData
		{
			int duration = 0;
			float rotate = 0;
		};

		struct translationFrameData {

			int duration = 0;
			gef::Vector2 XY = gef::Vector2(0, 0);

		};

		int current;
		std::string name = "";
		std::vector<translationFrameData> translations;
		int translationFramePos = 0;
		float translationFrameTime = 0;
		std::vector<rotateFrameData> rotations;
		int rotationFramePos = 0;
		float rotationFrameTime = 0;
	};

	std::vector<AnimationBoneData*> animation_bone_data;
};




struct Bone {
	std::string name = "";
	std::string parent = "";
	gef::Matrix33 transformMatrix;
	gef::Matrix33 local_transform;
	gef::Matrix33 worldBoneMatrix;
	Trans transform;
	unsigned int parentPos;
};
struct Slot {
	std::string name = "";
	std::string parent = "";
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

struct BoneArmature : Armature {
	std::vector<Bone*> bones;
	std::vector<SkinSlot*> skin;
	std::vector<Slot*> slot;
	std::vector<BoneAnimation*> animation;

};


struct SpriteJSONData {

	SpriteArmature* armiture = nullptr;
	TextureAtlas* texture_atlas = nullptr;
};

struct BoneJSONData {

	TextureAtlas* texture_atlas = nullptr;
	BoneArmature* bone_armiture = nullptr;



};



