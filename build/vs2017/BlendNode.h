#pragma once
#include <vector>
#include <graphics\skinned_mesh_instance.h>
#include <string>
class BlendTree;

namespace gef {
	class Platform;
	class Mesh;
	class Scene;
	class Skeleton;
}
class BlendNode
{
public:
	BlendNode(const gef::SkeletonPose* pose, BlendTree* tree);
	~BlendNode();
	virtual bool update(float dt);
	virtual bool process(float dt) = 0;

	void setInput(BlendNode* pose, int pos = 0);
	
	gef::SkeletonPose output_;
protected:

	std::vector<BlendNode*> inputs_;
	
	const gef::SkeletonPose* bindPose;
	std::string animation_name;
	std::string type;
	void cleanUp();
	BlendTree* tree_;

public:

	gef::SkeletonPose getOutput() { return output_; }
	void SetOutput(gef::SkeletonPose* pose) { output_ = *pose; }
};

