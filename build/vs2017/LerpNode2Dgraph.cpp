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
			std::map<gef::StringId, varibaleTable>::iterator &it = variable_table->find(nameId);

			if (it != variable_table->end())
			{
				variable_table->erase(it->first);
				variable_table->erase(blendABId);
				variable_table->erase(blendCDId);
				variable_table->erase(finalBlendId);
			}
		}

		
	}

}

LerpNode2Dgraph* LerpNode2Dgraph::create(const ImVec2& pos)
{

	LerpNode2Dgraph* node = new LerpNode2Dgraph();

	

	node->nodeName = "2dNodeLerp" + std::to_string(NodeId2D);
	node->nameId = gef::GetStringId(node->nodeName);

	node->init(node->nodeName.c_str(), pos, "clipA;clipB;clipC;clipD", "out", TYPE);

	node->blendABId = gef::GetStringId(node->nodeName + ".blendValAB");
	node->blendCDId = gef::GetStringId(node->nodeName + ".blendValCD");
	node->finalBlendId = gef::GetStringId(node->nodeName + ".blendValFinal");


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
	blendAB = variable_table->at(blendABId).floatData;
	blendCD = variable_table->at(blendCDId).floatData;
	finalBlend = variable_table->at(finalBlendId).floatData;

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



void LerpNode2Dgraph::setUp(std::map<gef::StringId, varibaleTable>* table, const gef::SkeletonPose* bind)
{

	if (!active)
	{
		//set up varable table
		variable_table = table;

		varibaleTable name;
		name.type = dataType::string;
		name.name = nodeName;
		variable_table->insert({ nameId, name });

		varibaleTable blendVal;
		blendVal.type = dataType::Real;
		blendVal.floatData = 0.5f;
		blendVal.max = 1.0f;
		blendVal.min = 0.0f;
		blendVal.name = nodeName + ".blendValAB";
		variable_table->insert({ blendABId, blendVal });
		blendVal.name = nodeName + ".blendValCD";
		variable_table->insert({ blendCDId, blendVal });
		blendVal.name = nodeName + ".blendValFinal";
		variable_table->insert({ finalBlendId, blendVal });

		setBind(bind);
		active = true;
	}
}

bool LerpNode2Dgraph::render(float)
{
	ImGui::Text("Requires four input\nProvides one output\nUse Variable table to to set blend values");
	return false;
}