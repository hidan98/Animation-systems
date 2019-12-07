#include "ikNodeGraph.h"
#include "ccd.h"


ikNodeGraph::ikNodeGraph() : CustomeNode(), player_(nullptr), effector_position_(nullptr)
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
		if (active)
		{
			

			std::vector<int> bone_indices;
			bone_indices.push_back(16); // left shoulder
			bone_indices.push_back(17); // left elbow
			bone_indices.push_back(18); // left wrist
			gef::SkeletonPose pose = node->getOutput();
			//ik_pose = graph->output->getOutput();
			CalculateCCD(pose, *player_, *effector_position_, positions);

			output_ = pose;
			return true;
		}

		
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

void ikNodeGraph::setup(gef::Platform* plat, const gef::SkeletonPose* bind, void* data)
{

}

void ikNodeGraph::setup(gef::Platform* plat,  const gef::SkeletonPose* bind, gef::SkinnedMeshInstance* play, gef::Vector4* pos)
{

	if (!active)
	{
		platform_ = plat;
		setBind(bind);
		player_ = play;
		effector_position_ = pos;
		active = true;
	}
}