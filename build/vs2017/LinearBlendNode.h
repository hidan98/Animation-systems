#pragma once
#include "BlendNode.h"
class LinearBlendNode: public BlendNode
{
public:
	//LinearBlendNode(gef::SkeletonPose* bind);
	LinearBlendNode(gef::SkeletonPose* bind, BlendNode* input1, BlendNode* input2, BlendTree* tree);
	~LinearBlendNode();
	
	bool process(float dt);
	void setBlendVal(float val);

private:

	float blendVal_;

	
};

