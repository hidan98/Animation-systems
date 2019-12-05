#include "nodeGraph.h"

#include "imguinodegrapheditor.h"



nodeGraph::nodeGraph()
{
}


nodeGraph::~nodeGraph()
{
}

static const char* MyNodeTypeNames[MNT_COUNT] = { "Color","Combine","Comment","Complex"
#						ifdef IMGUI_USE_AUTO_BINDING
,"Texture"
#						endif
,"Output"
};


//static ImGui::Node* MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& /*nge*/) {
////	switch (nt) {
////	case MNT_COLOR_NODE: return IColorno ColorNode::Create(pos);
////	case MNT_COMBINE_NODE: return CombineNode::Create(pos);
////	case MNT_COMMENT_NODE: return CommentNode::Create(pos);
////	case MNT_COMPLEX_NODE: return ComplexNode::Create(pos);
////	case MNT_OUTPUT_NODE: return OutputNode::Create(pos);
////#   ifdef IMGUI_USE_AUTO_BINDING
////	case MNT_TEXTURE_NODE: return TextureNode::Create(pos);
////#   endif //IMGUI_USE_AUTO_BINDING
////	default:
////		IM_ASSERT(true);    // Missing node type creation
////		return NULL;
////	}
////	return NULL;
//}

void nodeGraph::update()
{

	static ImGui::NodeGraphEditor nge;

	//if (nge.isInited())
	//{

	//	//nge.registerNodeTypes(MyNodeTypeNames, MNT_COUNT)

	//}

}