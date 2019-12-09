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
	gef::Scene* model = nullptr;
	gef::Skeleton* skel = nullptr;
	class gef::Mesh* mesh_ = nullptr;
	gef::SkinnedMeshInstance* skinnedMesh = nullptr;

};

class nodeGraph
{
public:
	nodeGraph(gef::Platform* plat, btDiscreteDynamicsWorld* world);
	~nodeGraph();

	//CustomeNode* getNode() { return static_cast<CustomeNode*>(nge.getNode(0)); }
	void init(gef::Vector4* pos, std::map<std::string, varibaleTable>* table);
	void update(float dt);

	CustomeNode* output;
	void updateOut(float dt) { output->update(dt, nodeGraphEdit); }
	currentData* getCurrent() { return &current; }

	currentData current;

	void cleanup();

	void render();

	std::vector<std::string>& getModels() { return modelNames; }

	void setUpModel(int pos);
private:
	//:MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor&);

	void getParent(int pos);

	//void setUpAnimations();
	const gef::SkeletonPose* bind_pose;	
	ImGui::NodeGraphEditor* nodeGraphEdit;
	
	gef::Platform* platform;


	std::vector<modelData> modelData_;

	btDiscreteDynamicsWorld* world_;
	std::string bulletPath;


	gef::Vector4* effector;

	std::map<gef::StringId, std::string> stringTable;

	std::map<std::string, varibaleTable>* variabe_table;
	std::vector<std::string> modelNames;
	gef::Scene* current_model;
	int bonePos;
	
	
};

