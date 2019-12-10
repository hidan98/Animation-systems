#pragma once
#include <string>
#include "maths/matrix44.h"
#include "maths/matrix33.h"
#include <vector>
#include <unordered_map>
#include "system/string_id.h"
struct SubTexture {
	float frameY;
	float y;
	float frameWidth;
	float frameX;
	float frameHeight;
	float width;
	float height;
	std::string name;
	gef::StringId nameId;
	float x;
	gef::Matrix33 subtextureTransform;

	float devideValX, devideValY;
};

struct TextureAtlas
{
	float width;
	float devideWidth;
	std::string imagePath;
	float height;
	float devideHeight;
	std::string name;
	//change from string
	std::unordered_map<gef::StringId, SubTexture*> sub_textures;
	gef::Matrix33 baseTransform;
};

struct Trans {
	float x = 0;
	float y = 0;
	float sk = 0;

};

struct SkinSlot {
	std::string name = "";
	gef::StringId nameId;
	gef::StringId displayId;
	std::string display_name = "";
	Trans transform;
	gef::Matrix33 transformMatrix;
};


struct Display
{
	std::vector<std::string> display_name;
	std::vector<gef::StringId> display_name_id;
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


struct rotateFrameData
{
	float duration = 0;
	float startTime = 0;
	float rotate = 0;
};

struct translationFrameData {

	float duration = 0;
	float startTime = 0;
	gef::Vector2 XY = gef::Vector2(0, 0);

};
struct AnimationBoneData {	

	int current;
	std::string name = "";
	gef::StringId nameId;
	std::vector<translationFrameData> translations;
	int translationFramePos = 0;
	float translationFrameTime = 0;
	std::vector<rotateFrameData> rotations;
	int rotationFramePos = 0;
	float rotationFrameTime = 0;
	float time = 0;

	float duration = 0;
};


struct BoneAnimation : Animation
{
	std::vector<AnimationBoneData*> animation_bone_data;
};

struct Bone {
	std::string name = "";
	gef::StringId nameStringId= 0;
	std::string parent = "";
	gef::StringId parentStringId = 0;
	gef::Matrix33 transformMatrix;
	gef::Matrix33 local_transform;
	gef::Matrix33 worldBoneMatrix;
	Trans transform;
	unsigned int parentPos;
};


struct Slot {
	std::string name = "";
	gef::StringId nameStringId;
	std::string parent = "";
	gef::StringId parentStringId;
	gef::Matrix33 finalTransform;

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

struct BoneDataStruct {
	std::unordered_map<gef::StringId, Bone*> bonesMap;
	std::vector<Bone*> bonesVec;
};

struct BoneArmature : Armature {
	BoneDataStruct boneData;
	//std::vector<Bone*> bones;
	std::unordered_map <gef::StringId, SkinSlot*> skin;
	//std::vector<SkinSlot*> skin;
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



