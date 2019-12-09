#include "BlendNodeGraph.h"
#include "graphics/skinned_mesh_instance.h"


BlendNodeGraph::BlendNodeGraph() :CustomeNode()
{
	active = false;
}


BlendNodeGraph::~BlendNodeGraph()
{

	//if the variable table sill exhists remove variables from it
	if (variable_table)
	{
		std::map<std::string, varibaleTable>::iterator &it = variable_table->find(Nodename);

		if (it != variable_table->end())
		{
			variable_table->erase(it->first);
			variable_table->erase(Nodename + ".blendVal");

			variable_table->erase(Nodename + ".partalBlendToggle");
		}
	}
	
}


bool BlendNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	CustomeNode* one = nullptr;
	CustomeNode* two = nullptr;

	//loop through inputs and set casted values, if an input node is not valid return false
	for (int i = 0; i < InputsCount; i++)
	{
		CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, i));
		if (!node)
			return false;

		if (!one)
			one = node;
		else if (!two)
			two = node;
	}

	//grab variables from table
	blendVal = variable_table->at( Nodename + ".blendVal").floatData;
	partalBlend = variable_table->at(Nodename + ".partalBlendToggle").toggle;

	//if the partal blend has bee selected 
	if (partalBlend)
	{
		//perform it
		output_.customeLinearBlend(one->getOutput(), two->getOutput(), boneInd, blendVal);
	}
	else//otherwise perform normal blend
		output_.Linear2PoseBlend(one->getOutput(), two->getOutput(), blendVal);
	return true;
}


BlendNodeGraph* BlendNodeGraph::create(const ImVec2& pos)
{
	BlendNodeGraph* node = new BlendNodeGraph();

	node->init("Ouput node", pos, "ch1;ch2", "out", TYPE);

	node->blendVal = 0.5f;
	node->partalBlend = false;

	node->Nodename = "blendNode" + std::to_string(idCount);
	idCount++;

	

	//node->fields.add
	return node;
}

void BlendNodeGraph::setup(std::map<std::string, varibaleTable>* table, const gef::SkeletonPose* bind)
{
	if (!active)
	{
		variable_table = table;
		setBind(bind);
		active = true;


		//set up variables for table
		varibaleTable name;
		name.type = dataType::string;
		name.name = Nodename;
		variable_table->insert({ Nodename, name });

		varibaleTable blendVal;
		blendVal.type = dataType::Real;
		blendVal.floatData = 0.5f;
		blendVal.max = 1.0f;
		blendVal.min = 0.0f;

		variable_table->insert({Nodename + ".blendVal", blendVal });

		varibaleTable toggle;
		toggle.type = dataType::boolean;
		toggle.toggle = false;

		variable_table->insert({ Nodename + ".partalBlendToggle", toggle });
	}

	

	

	

}

