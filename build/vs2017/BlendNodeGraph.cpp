#include "BlendNodeGraph.h"



BlendNodeGraph::BlendNodeGraph() :CustomeNode()
{
	active = false;
}


BlendNodeGraph::~BlendNodeGraph()
{
}


bool BlendNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	CustomeNode* one = nullptr;
	CustomeNode* two = nullptr;

	for (int i = 0; i < InputsCount; i++)
	{
		CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, i));
		if (!node)
			return false;

		if (!one)
			one = node;
		else if (!two)
			two = node;
	}

	output_.Linear2PoseBlend(one->getOutput(), two->getOutput(), blendVal);
	return true;
}


BlendNodeGraph* BlendNodeGraph::create(const ImVec2& pos)
{
	BlendNodeGraph* node = new BlendNodeGraph();

	node->init("Ouput node", pos, "ch1;ch2", "out", TYPE);

	node->fields.addField(&node->blendVal, 1, "Blend Value", "How much should it blend");
	node->blendVal = 0.5f;
	//node->fields.add
	return node;
}

void BlendNodeGraph::setup(gef::Platform* plat, gef::SkeletonPose* bind, void* data)
{
	if (!active)
	{
		setBind(bind);
		SetOutput(bind);
		active = true;
	}
	
}