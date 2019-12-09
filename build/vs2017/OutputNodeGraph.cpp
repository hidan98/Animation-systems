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

void OutputNodeGraph::setup(gef::Platform* plat, const gef::SkeletonPose* bind)
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
	OutputNodeGraph* node = new OutputNodeGraph();

	//set name, position,, number of inputs and type
	node->init("Ouput node", pos, "ch1","", MNT_OUTPUT_NODE);


	//node->fields.add
	return node;
}

bool OutputNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	//grab input node and cast
	CustomeNode* input = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, 0));
	if (input)
	{
		//if
		output_ = input->getOutput();
		
		return true;

	}
	else
		output_ = *bindPose;
	return false;

}