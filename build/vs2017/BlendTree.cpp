#include "BlendTree.h"



BlendTree::BlendTree()
{
}


BlendTree::~BlendTree()
{
	cleanUp();
}

bool BlendTree::update(float dt)
{
	return outPut_->update(dt);
}

void BlendTree::init(gef::SkeletonPose* pose_, BlendNode* node)
{

	outPut_ = new OutputNode(pose_, node, this);
}

void BlendTree::cleanUp()
{
	delete outPut_;
	outPut_ = nullptr;

}