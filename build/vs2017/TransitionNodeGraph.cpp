#include "TransitionNodeGraph.h"
#include "ClipNodeGraph.h"
static int transNodeid = 0;

TransitionNodeGraph::TransitionNodeGraph()
{
	time = 0;
}


TransitionNodeGraph::~TransitionNodeGraph()
{
}

TransitionNodeGraph* TransitionNodeGraph::create(const ImVec2& pos)
{
	TransitionNodeGraph* node = new TransitionNodeGraph();

	//node->init("transNode", pos, "in1;in2", "out", TYPE);

	node->nodeName = "transNode" + std::to_string(transNodeid);
	transNodeid++;

	return node;
}

bool TransitionNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	ImGui::Node* one = editor->getInputNodeForNodeAndSlot(this, 0);
	ImGui::Node* two = editor->getInputNodeForNodeAndSlot(this, 1);

	ClipNodeGraph* clipOne = nullptr;
	ClipNodeGraph* clipTwo = nullptr;
	if (one)
	{
		if (one->getType() == clipNode)
		{
			clipOne = static_cast<ClipNodeGraph*>(one);
		}
	}

	if (two)
	{
		if (one->getType() == clipNode)
		{
			clipTwo = static_cast<ClipNodeGraph*>(two);
		}
	}

	/*if (clipOne && clipTwo)
	{
		if (smooth)
		{
			timer += dt;

			time = timer/ ()
			
		}
		output_.Linear2PoseBlend(clipOne->getOutput(), clipTwo->getOutput(), time);

		return true;
	}8*/
	return false;

}