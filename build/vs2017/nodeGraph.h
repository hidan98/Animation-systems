#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <platform/d3d11/system/platform_d3d11.h>
#include <platform\d3d11\input\keyboard_d3d11.h>

#include "OutputNodeGraph.h"
#include "ClipNodeGraph.h"

#include "graphics\skinned_mesh_instance.h"
#include "btBulletDynamicsCommon.h"

struct animationData
{
	std::string name;
	gef::Scene* scene_;
	gef::Animation* animation_;
	std::string bulletPath;

};

class nodeGraph
{
public:
	nodeGraph(gef::SkeletonPose pose, gef::Platform* plat, btDiscreteDynamicsWorld* world);
	~nodeGraph();

	//CustomeNode* getNode() { return static_cast<CustomeNode*>(nge.getNode(0)); }
	
	void update(float dt);

	CustomeNode* output;
	void updateOut(float dt) { output->update(dt, temp_); }
	
private:
	//:MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor&);

	void setUpAnimations();
	gef::SkeletonPose bind_pose;
	
	ImGui::NodeGraphEditor* temp_;
	

	gef::Platform* platform;

	std::vector<animationData> animations_;
	std::vector<std::vector<animationData*>> animations;

	animationData* loadAnimation(std::string path);

	btDiscreteDynamicsWorld* world_;
	std::string bulletPath;
};

