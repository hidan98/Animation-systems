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
		//if the input is valud get the ouput from the input node and set it to thte output
		output_ = input->getOutput();
		
		return true;

	}
	else//if we dont have an input set the output to the bind pose
		output_ = *bindPose;
	return false;

}

bool OutputNodeGraph::render(float)
{
	ImGui::Text("Only one output instance can exhist,\n require only one input");
	return false;
}