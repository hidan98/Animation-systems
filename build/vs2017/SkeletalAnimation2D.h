#pragma once
#include "AnimationData.h"
#include <map>
#include "graphics\sprite.h"



namespace gef {

	class Platform;
	class SpriteRenderer;
}
class SkeletalAnimation2D
{
public:
	SkeletalAnimation2D();
	~SkeletalAnimation2D();

	void init(const char* tex, const char* ske, const char* image, gef::Platform& plat);
	void update(float dt, gef::Vector2 screenPos);

	inline BoneJSONData* getData() { return json_data_; }
	void render(gef::SpriteRenderer* renderer);
	inline void setAnimationNum(int num) { animationNum = num; }

	void cleanUp();

private:

	void updateBoneTransform(BoneDataStruct bones);
	void updateFinalTransform(std::vector<Slot*> slot);

	void updateBones(std::vector<BoneAnimation*> animation, std::map<gef::StringId, Bone*> bones, float dt);

	void spriteSetup(SubTexture* sub, gef::Vector2 screenPos, gef::Sprite* sprite);

	float getRotLerp(AnimationBoneData* data, float dt);
	gef::Vector2 getTransLerp(AnimationBoneData* data, float dt);

	float lerpRot(float start, float diff, float t);

	BoneJSONData* json_data_;

	std::vector<gef::Sprite*> sprites;


	gef::Texture* sprite_texture;

	int animationNum;

};

