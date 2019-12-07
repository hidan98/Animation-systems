#pragma once
#include "imguinodegrapheditor.h"
#include <vector>
#include "graphics\skinned_mesh_instance.h"
#include "system/platform.h"
class CustomeNode : public ImGui::Node
{
public:
	CustomeNode();
	~CustomeNode();

	virtual bool update(float dt, ImGui::NodeGraphEditor* temp);
	virtual bool process(float dt, ImGui::NodeGraphEditor* editor) = 0;
	virtual void setup(gef::Platform* plat, const gef::SkeletonPose* bind, void* = nullptr) = 0;

	gef::SkeletonPose getOutput() { return output_; }
	void SetOutput(gef::SkeletonPose* pose) { output_ = *pose; }
	inline void setBind(const gef::SkeletonPose* pose) { bindPose = pose; output_ = *pose; }
	inline void setPlatform(gef::Platform* plat) { platform_ = plat; }
protected:
	//gef::SkeletonPose* bind_pose;
	//std::vector<CustomeNode*> inputs_;
	const gef::SkeletonPose* bindPose;
	gef::SkeletonPose output_;
	gef::Platform* platform_;

	bool active;
};

