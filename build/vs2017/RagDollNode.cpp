#include "RagDollNode.h"


RagDollNode::RagDollNode() : CustomeNode(), ragdoll_(nullptr), world_(nullptr)
{
	active = false;
}


RagDollNode::~RagDollNode()
{

	//// remove the rigidbodies from the dynamics world and delete them
	//for (int i = world_->getNumCollisionObjects() - 1; i >= 0; i--)
	//{
	//	btCollisionObject * obj = world_->getCollisionObjectArray()[i];
	//	btRigidBody * body = btRigidBody::upcast(obj);
	//	if (body && body->getMotionState())
	//	{
	//		delete body->getMotionState();
	//	}
	//	world_->removeCollisionObject(obj);
	//	delete obj;
	//}

	delete ragdoll_;
	ragdoll_ = nullptr;
}


RagDollNode* RagDollNode::create(const ImVec2& pos)
{
	RagDollNode* node = new RagDollNode();

	node->init("RagDoll node", pos, "in", "out", TYPE);

	node->fields.addField(&node->activateRagDoll, "Turn on rag doll");
	node->activateRagDoll = false;
	//node->fields.add
	return node;
}
bool RagDollNode::process(float dt, ImGui::NodeGraphEditor* editor)
{
	CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, 0));
	if (node)
	{
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
		return true;
	}

	return false;
}
void RagDollNode::setup(gef::Platform* plat, const gef::SkeletonPose* bind, btDiscreteDynamicsWorld* world, std::string path)
{
	if (!active)
	{
		if (bind->skeleton())
		{
			ragdoll_ = new Ragdoll();
			ragdoll_->set_scale_factor(0.01f);


			ragdoll_->Init(*bind, world, path.c_str());
			setBind(bind);
			//SetOutput(bind);
			active = true;
			world_ = world;
		}
		
	}
}
void RagDollNode::setup(gef::Platform* plat, const gef::SkeletonPose* bind)
{
	

}
