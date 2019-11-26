#pragma once
#include "AnimationData.h"
#include <map>
namespace gef {

	class Platform;
}
class SkeletalAnimation2D
{
public:
	SkeletalAnimation2D();
	~SkeletalAnimation2D();

	void init(const char* tex, const char* ske, const char* image, gef::Platform& plat);

	void update();

private:

	void updateBoneTransform(std::unordered_map<gef::StringId, Bone*> bones);
	void updateFinalTransform(std::vector<Slot*> slot);

	BoneJSONData* json_data_;

};

