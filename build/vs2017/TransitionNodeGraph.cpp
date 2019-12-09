#include "TransitionNodeGraph.h"

static int transNodeid = 0;

TransitionNodeGraph::TransitionNodeGraph()
{
}


TransitionNodeGraph::~TransitionNodeGraph()
{
}

TransitionNodeGraph* TransitionNodeGraph::create(const ImVec2& pos)
{
	TransitionNodeGraph* node = new TransitionNodeGraph();

	node->init("transNode", pos, "in1;in2", "out", TYPE);

	node->nodeName = "transNode" + std::to_string(transNodeid);
	transNodeid++;

	return node;
}

bool TransitionNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{

	if (frozen)
	{

	}
	return true;
}