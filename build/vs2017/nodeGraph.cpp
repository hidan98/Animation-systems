#include "nodeGraph.h"

#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"


nodeGraph::nodeGraph(gef::SkeletonPose pose): output(NULL)
{
	bind_pose = pose;
}


nodeGraph::~nodeGraph()
{
}

static const char* MyNodeTypeNames[MNT_COUNT] = { "Color","Combine","Comment","Clip"
#						ifdef IMGUI_USE_AUTO_BINDING
,"Texture"
#						endif
,"Output"
};


static ImGui::Node* MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& nge) {
	switch (nt) {
	case MNT_OUTPUT_NODE: return OutputNodeGraph::create(pos);
	case clipNode: {ClipNodeGraph* temp = ClipNodeGraph::create(pos);
		//temp->setPlatform()
		
		return temp; }
	/*case MNT_COMMENT_NODE: return CommentNode::Create(pos);
	case MNT_COMPLEX_NODE: return ComplexNode::Create(pos);
	case MNT_OUTPUT_NODE: return OutputNode::Create(pos);*/
#   ifdef IMGUI_USE_AUTO_BINDING
	case MNT_TEXTURE_NODE: return TextureNode::Create(pos);
#   endif //IMGUI_USE_AUTO_BINDING
	default:
		IM_ASSERT(true);    // Missing node type creation
		return NULL;
	}
	
	return NULL;
}


void nodeGraph::update(float dt)
{

	static ImGui::NodeGraphEditor nge;

	if (nge.isInited())
	{

		nge.registerNodeTypes(MyNodeTypeNames, MNT_COUNT, MyNodeFactory, NULL, -1);

		nge.registerNodeTypeMaxAllowedInstances(MNT_OUTPUT_NODE, 1);
		output =  static_cast<OutputNodeGraph*>(nge.addNode(MNT_OUTPUT_NODE, ImVec2(40, 50)));
		output->setBind(&bind_pose);
		/*nge.show_style_editor = true;
		nge.show_load_save_buttons = true;*/
	}
	
	
	ImVector<ImGui::Node*> yes;
	nge.getAllNodesOfType(clipNode, &yes);


	nge.getAllNodesOfType(MNT_OUTPUT_NODE, &yes);
	CustomeNode* temp = static_cast<CustomeNode*>(yes[0]);
	temp->update(dt, &nge);
	temp_ = &nge;
	nge.render();
	
}