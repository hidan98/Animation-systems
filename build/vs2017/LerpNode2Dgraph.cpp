#include "LerpNode2Dgraph.h"



LerpNode2Dgraph::LerpNode2Dgraph()
{
}


LerpNode2Dgraph::~LerpNode2Dgraph()
{
}

LerpNode2Dgraph* LerpNode2Dgraph::create(const ImVec2& pos)
{

	LerpNode2Dgraph* node = new LerpNode2Dgraph();

	node->init("Ouput node", pos, "clipA;clipB;clipC;clipD", "out", TYPE);

	node->fields.addField(&node->blendAB, 1, "Blend Value", "How much should it blend between clip A and B");
	node->blendAB = 0.5f;

	node->fields.addField(&node->blendCD, 1, "Blend Value", "How much should it blend between clip C and D");
	node-> blendCD = 0.5f;

	node->fields.addField(&node->finalBlend, 1, "Blend Value", "How much should it blend between the resulting clips");
	node->finalBlend = 0.5f;
	//node->fields.add
	return node;

}

bool LerpNode2Dgraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	CustomeNode* clipA = nullptr;
	CustomeNode* clipB = nullptr;
	CustomeNode* clipC = nullptr;
	CustomeNode* clipD = nullptr;
	for (int i = 0; i < InputsCount; i++)
	{
		CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, i));
		if (!node)
			return false;

		if (!clipA)
			clipA = node;
		else if (!clipB)
			clipB = node;
		else if (!clipC)
			clipC = node;
		else if (!clipD)
			clipD = node;
		else
			return false;
	}

	gef::SkeletonPose AB;
	AB = *bindPose;
	AB.Linear2PoseBlend(clipA->getOutput(), clipB->getOutput(), blendAB);
	gef::SkeletonPose CD;
	CD = *bindPose;
	CD.Linear2PoseBlend(clipC->getOutput(), clipD->getOutput(), blendCD);

	output_.Linear2PoseBlend(AB, CD, finalBlend);
	return true;
}
