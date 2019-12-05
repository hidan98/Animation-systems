#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <platform/d3d11/system/platform_d3d11.h>
#include <platform\d3d11\input\keyboard_d3d11.h>

#include "OutputNodeGraph.h"
#include "ClipNodeGraph.h"

#include "graphics\skinned_mesh_instance.h"


class nodeGraph
{
public:
	nodeGraph(gef::SkeletonPose pose);
	~nodeGraph();

	
	void update(float dt);
	CustomeNode* output;
private:
	//:MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor&);

	gef::SkeletonPose bind_pose;
	
	
};

