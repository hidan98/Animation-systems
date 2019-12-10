#pragma once
#include "imguinodegrapheditor.h"
#include <vector>
#include "graphics\skinned_mesh_instance.h"
#include "system/platform.h"
#include <map>
#include "Animation_Utils.h"
class CustomeNode : public ImGui::Node
{
public:
	CustomeNode();
	~CustomeNode();

	virtual bool update(float dt, ImGui::NodeGraphEditor* temp);
	virtual bool process(float dt, ImGui::NodeGraphEditor* editor) = 0;
	inline virtual void setup(gef::Platform* plat, const gef::SkeletonPose* bind, std::map<std::string, varibaleTable>* table) { if (!active) { platform_ = plat; bindPose = bind; output_ = *bind; variable_table = table; active = true; } }

	gef::SkeletonPose getOutput() { return output_; }
	void SetOutput(gef::SkeletonPose* pose) { output_ = *pose; }
	inline void setBind(const gef::SkeletonPose* pose) { bindPose = pose; output_ = *pose; }
	inline void setPlatform(gef::Platform* plat) { platform_ = plat; }
protected:


	const gef::SkeletonPose* bindPose;
	gef::SkeletonPose output_;
	gef::Platform* platform_;

	std::map<std::string, varibaleTable>* variable_table;

	bool active;
};

