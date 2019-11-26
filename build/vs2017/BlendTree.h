#pragma once
#include "OutputNode.h"
#include <string>
class BlendTree
{
public:
	BlendTree();
	~BlendTree();

	bool update(float dt);
	void init(gef::SkeletonPose* pose_, BlendNode* node);
	void cleanUp();


	OutputNode* outPut_;

//private:

	std::map<std::string, float> variable_table;
	

};

