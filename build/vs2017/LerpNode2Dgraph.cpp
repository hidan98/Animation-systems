#include "LerpNode2Dgraph.h"



LerpNode2Dgraph::LerpNode2Dgraph() : CustomeNode()
{
	active = false;
}


LerpNode2Dgraph::~LerpNode2Dgraph()
{
	if (variable_table)
	{
		if (variable_table->size() > 0)
		{
			std::map<std::string, varibaleTable>::iterator &it = variable_table->find(nodeName);

			if (it != variable_table->end())
			{
				variable_table->erase(it->first);
				variable_table->erase(nodeName + ".blendValAB");
				variable_table->erase(nodeName + ".blendValCD");
				variable_table->erase(nodeName + ".blendValFinal");
			}
		}

		
	}

}

LerpNode2Dgraph* LerpNode2Dgraph::create(const ImVec2& pos)
{

	LerpNode2Dgraph* node = new LerpNode2Dgraph();

	node->init("2D node lerp", pos, "clipA;clipB;clipC;clipD", "out", TYPE);

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
	//loop through inputs setting up the custome nodes
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
	//get data from variable table 
	blendAB = variable_table->at(nodeName + ".blendValAB").floatData;
	blendCD = variable_table->at(nodeName + ".blendValCD").floatData;
	finalBlend = variable_table->at(nodeName + ".blendValFinal").floatData;

	//blend clip a and b
	AB.Linear2PoseBlend(clipA->getOutput(), clipB->getOutput(), blendAB);
	gef::SkeletonPose CD;
	CD = *bindPose;
	//blend clip c and d
	CD.Linear2PoseBlend(clipC->getOutput(), clipD->getOutput(), blendCD);

	//blend the resulting two clips 
	output_.Linear2PoseBlend(AB, CD, finalBlend);
	return true;
}



void LerpNode2Dgraph::setUp(std::map<std::string, varibaleTable>* table, const gef::SkeletonPose* bind)
{

	if (!active)
	{
		//set up varable table
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
