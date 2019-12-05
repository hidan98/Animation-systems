#pragma once
#include "imguinodegrapheditor.h"
#include <vector>
#include "graphics\skinned_mesh_instance.h"

class CustomeNode : public ImGui::Node
{
public:
	CustomeNode();
	~CustomeNode();

	virtual bool update(float dt);
	virtual bool process(float dt) = 0;

	gef::SkeletonPose getOutput() { return output_; }
	void SetOutput(gef::SkeletonPose* pose) { output_ = *pose; }
	inline void setBind(const gef::SkeletonPose* pose) { bindPose = pose; }
protected:
	gef::SkeletonPose* bind_pose;
	//std::vector<CustomeNode*> inputs_;
	const gef::SkeletonPose* bindPose;
	gef::SkeletonPose output_;
};

