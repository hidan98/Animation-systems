#pragma once
#include "BlendNode.h"

namespace gef {
	class Platform;
	class Mesh;
	class Skeleton;
}
class BlendTree;


class OutputNode : public BlendNode
{
public:
	OutputNode(const gef::SkeletonPose* pose, BlendNode* node, BlendTree* tree);
	~OutputNode();
	bool process(float dt);

//	void setInput(BlendNode* node, int pos = 0);
};

