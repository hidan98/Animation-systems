#include "BlendNode.h"

#include "BlendTree.h"


BlendNode::BlendNode(const gef::SkeletonPose* pose, BlendTree* tree) : bindPose(pose), tree_(tree)
{
	output_ = *bindPose;
}


BlendNode::~BlendNode()
{
	cleanUp();
}

void BlendNode::cleanUp()
{

	for (int i = 0; i < inputs_.size(); i++)
	{
		delete inputs_[i];
		inputs_[i] = nullptr;
	}

}

bool BlendNode::update(float dt)
{
	bool all_inputs = true;

	for (auto it : inputs_)
	{
		bool input_valid = false;

		if (it)
			input_valid = it->update(dt);

		if (!input_valid)
		{
			all_inputs = false;
			return false;
		}
	}

	bool output_valid = false;

	if (all_inputs)
		output_valid = process(dt);

	return output_valid;

			
}


void BlendNode::setInput(BlendNode* node, int pos)
{
	if(inputs_.size() -1 <= pos)
		inputs_[pos] = node;
	else
	{
		inputs_.resize(pos);
		inputs_[pos] = node;
	}
}