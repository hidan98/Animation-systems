#include "LinearBlendNode.h"


LinearBlendNode::LinearBlendNode(gef::SkeletonPose* bind, BlendNode* input1, BlendNode* input2, BlendTree* tree) :BlendNode(bind, tree), blendVal_(0)
{
	inputs_.push_back(input1);
	inputs_.push_back(input2);
	type = "linearBlend";
}


LinearBlendNode::~LinearBlendNode()
{
}

bool LinearBlendNode::process(float dt)
{

	output_.Linear2PoseBlend(inputs_[0]->getOutput(), inputs_[1]->getOutput(), blendVal_);
	return true;
}

void LinearBlendNode::setBlendVal(float val)
{
	blendVal_ = val;
}