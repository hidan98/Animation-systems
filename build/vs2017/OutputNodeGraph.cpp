#include "OutputNodeGraph.h"
#include "nodeGraph.h"
#include "imguinodegrapheditor.h"

OutputNodeGraph::OutputNodeGraph() : CustomeNode()
{
	active = false;
}


OutputNodeGraph::~OutputNodeGraph()
{
}

void OutputNodeGraph::setup(gef::Platform* plat, const gef::SkeletonPose* bind, void* data)
{
	if (!active)
	{
		platform_ = plat;
		bindPose = bind;
		active = true;
		output_ = *bindPose;
	}
	
}

OutputNodeGraph* OutputNodeGraph::create(const ImVec2& pos)
{
	OutputNodeGraph* node = (ThisClass*)ImGui::MemAlloc(sizeof(ThisClass)); IM_PLACEMENT_NEW(node) ThisClass();

	node->init("Ouput node", pos, "ch1","", MNT_OUTPUT_NODE);


	//node->fields.add
	return node;
}

bool OutputNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	
	CustomeNode* input = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, 0));
	if (input)
	{
		output_ = input->getOutput();
		return true;

	}
	else
		output_ = *bindPose;
	return false;

}