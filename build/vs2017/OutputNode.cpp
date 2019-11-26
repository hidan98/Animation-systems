#include "OutputNode.h"
#include "BlendTree.h"

OutputNode::OutputNode(const gef::SkeletonPose* pose, BlendNode* node, BlendTree* tree) : BlendNode(pose, tree)
{
	inputs_.push_back(node);
	type = "outPut";
}


OutputNode::~OutputNode()
{
}

bool OutputNode::process(float dt)
{
	if (inputs_[0])
	{
		this->output_ = inputs_[0]->getOutput();
		return true;
	}
	else
	{
		output_ = *bindPose;
	}

	return false;
}
