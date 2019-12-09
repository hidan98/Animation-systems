#include "LerpNode2Dgraph.h"



LerpNode2Dgraph::LerpNode2Dgraph() : CustomeNode()
{
	active = false;
}


LerpNode2Dgraph::~LerpNode2Dgraph()
{
}

LerpNode2Dgraph* LerpNode2Dgraph::create(const ImVec2& pos)
{

	LerpNode2Dgraph* node = new LerpNode2Dgraph();

	node->init("2D node lerp", pos, "clipA;clipB;clipC;clipD", "out", TYPE);

	node->fields.addField(&node->blendAB, 1, "Blend Value", "How much should it blend between clip A and B");
	node->blendAB = 0.5f;

	node->fields.addField(&node->blendCD, 1, "Blend Value", "How much should it blend between clip C and D");
	node-> blendCD = 0.5f;

	node->fields.addField(&node->finalBlend, 1, "Blend Value", "How much should it blend between the resulting clips");
	node->finalBlend = 0.5f;

	node->nodeName = "2dNodeLerp" + std::to_string(NodeId2D);

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
	blendAB = variable_table->at(nodeName + ".blendValAB").floatData;
	blendCD = variable_table->at(nodeName + ".blendValCD").floatData;
	finalBlend = variable_table->at(nodeName + ".blendValFinal").floatData;
	AB.Linear2PoseBlend(clipA->getOutput(), clipB->getOutput(), blendAB);
	gef::SkeletonPose CD;
	CD = *bindPose;
	CD.Linear2PoseBlend(clipC->getOutput(), clipD->getOutput(), blendCD);

	output_.Linear2PoseBlend(AB, CD, finalBlend);
	return true;
}



void LerpNode2Dgraph::setUp(std::map<std::string, varibaleTable>* table, const gef::SkeletonPose* bind)
{

	if (!active)
	{
		variable_table = table;

		varibaleTable name;
		name.type = dataType::string;
		name.name = nodeName;
		variable_table->insert({ nodeName, name });

		varibaleTable blendVal;
		blendVal.type = dataType::Real;
		blendVal.floatData = 0.5f;
		blendVal.max = 1.0f;
		blendVal.min = 0.0f;

		variable_table->insert({ nodeName + ".blendValAB", blendVal });
		variable_table->insert({ nodeName + ".blendValCD", blendVal });
		variable_table->insert({ nodeName + ".blendValFinal", blendVal });

		setBind(bind);
		active = true;
	}
}
