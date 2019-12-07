#include "ikNodeGraph.h"
#include "ccd.h"


ikNodeGraph::ikNodeGraph()
{
}


ikNodeGraph::~ikNodeGraph()
{
}

ikNodeGraph* ikNodeGraph::create(const ImVec2& pos)
{
	ikNodeGraph* node = new ikNodeGraph();

	node->init("Ik Node", pos, "in", "out", TYPE);

	//node->fields.addField(&node->activateRagDoll, "Turn on rag doll");
	//node->activateRagDoll = false;
	//node->fields.add
	return node;
}

bool ikNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{


	CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, 0));
	if (node)
	{
		/*gef::SkeletonPose* ik_pose = &node->getOutput();
		CalculateCCD(ik_pose, )
		

		if (!activateRagDoll)
		{
			ragdoll_->set_pose(node->getOutput());
			ragdoll_->UpdateRagdollFromPose();
			output_ = node->getOutput();
		}
		else
		{
			ragdoll_->UpdatePoseFromRagdoll();
			output_ = ragdoll_->pose();
		}
		return true;*/
	}

	return false;
}

void ikNodeGraph::setup(gef::Platform* plat, gef::SkeletonPose* bind, void* data)
{

}