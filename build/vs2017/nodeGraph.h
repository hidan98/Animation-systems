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

#include <map>

struct modelData
{
	std::string name;
	gef::Scene* scene_ = nullptr;
	std::map<gef::StringId, gef::Animation*> animations_;
	// std::vector<gef::Animation*> animations_;
	//gef::Animation* animation_;
	std::string bulletPath;

};

struct currentData {
	gef::Scene* model;
	gef::Skeleton* skel;

};

class nodeGraph
{
public:
	nodeGraph(const gef::SkeletonPose* pose, gef::Platform* plat, btDiscreteDynamicsWorld* world, gef::SkinnedMeshInstance* mesh);
	~nodeGraph();

	//CustomeNode* getNode() { return static_cast<CustomeNode*>(nge.getNode(0)); }
	void init(gef::Vector4* pos);
	void update(float dt);

	CustomeNode* output;
	void updateOut(float dt) { output->update(dt, temp_); }
	
	currentData current;
private:
	//:MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor&);

	void getParent(int pos);
	//void setUpAnimations();
	const gef::SkeletonPose* bind_pose;
	
	ImGui::NodeGraphEditor* temp_;
	
	std::vector<bool> selected;
	gef::Platform* platform;

	/*std::vector<animationData> animations_;
	std::vector<std::vector<animationData*>> animations;

	animationData* loadAnimation(std::string path);*/

	std::vector<modelData*> modelData_;

	btDiscreteDynamicsWorld* world_;
	std::string bulletPath;

	gef::SkinnedMeshInstance* skinned_mesh_player;
	gef::Vector4* effector;

	std::map<gef::StringId, std::string> stringTable;

	gef::Scene* current_model;
	int bonePos;
	
	
};

