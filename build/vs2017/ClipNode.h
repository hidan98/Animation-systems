#pragma once
#include "BlendNode.h"
#include <graphics/skinned_mesh_instance.h>
#include <animation/skeleton.h>
#include <animation/animation.h>

namespace gef {
	class Pltform;
	class Mesh;
	class Skeleton;
	class Scene;
}
class ClipNode : public BlendNode
{
public:
	ClipNode(const gef::SkeletonPose* pose, BlendTree *tree, const char* name);
	~ClipNode();
	bool update(float dt) override;



private:

	float animTime_;
	bool looping;
	float playBackSpeed_;
	gef::Animation* clip_;

public:
	//void setClip(gef::Animation* animation) { clip_ = animation; }
	void setUpClip(gef::Animation* animmation, bool loop = false, float playback = 1.0f) {
		clip_ = animmation; looping = loop; playBackSpeed_ = playback;
	}
	bool process(float dt);

};

