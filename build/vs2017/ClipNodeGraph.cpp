#include "ClipNodeGraph.h"



ClipNodeGraph::ClipNodeGraph()
{
}


ClipNodeGraph::~ClipNodeGraph()
{
}
ClipNodeGraph* ClipNodeGraph::create(const ImVec2& pos)
{
	ThisClass* node = (ThisClass*)ImGui::MemAlloc(sizeof(ThisClass)); IM_PLACEMENT_NEW(node) ThisClass();

	// 2) main init
	node->init("Clip node", pos, "", "clip", TYPE);

	
	// 3) init fields ( this uses the node->fields variable; otherwise we should have overridden other virtual methods (to render and serialize) )
	node->fields.addField(&node->speed, 1, "Playback speed", "Speed of the animiation");
	node->speed = 1.0f;
	


	// 4) set (or load) field values
	//node->Color = ImColor(255, 255, 0, 255);

	return node;
}


void ClipNodeGraph::update()
{
//	this->getNodeGraphEditor().getInputNodeForNodeAndSlot();

}