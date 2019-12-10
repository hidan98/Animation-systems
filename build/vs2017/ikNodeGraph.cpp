#include "ikNodeGraph.h"
#include "ccd.h"


ikNodeGraph::ikNodeGraph() : CustomeNode(), player_(nullptr), effector_position_(nullptr)
{
	position = gef::Vector4(0, 0, 0);
	fix = false;
}


ikNodeGraph::~ikNodeGraph()
{
	if (variable_table)
	{
		if (variable_table->size() > 0)
		{
			std::map<std::string, varibaleTable>::iterator &it = variable_table->find(nodeName);

			if (it != variable_table->end())
			{
				variable_table->erase(it->first);
				variable_table->erase(nodeName + ".fix");
				//variable_table->erase(nodeName + ".looping");

			}
		}

		
	}
}

ikNodeGraph* ikNodeGraph::create(const ImVec2& pos)
{
	ikNodeGraph* node = new ikNodeGraph();

	node->nodeName = "IKNode";// +std::to_string(ikNodeID);
	node->init("Ik Node", pos, "in", "out", TYPE);

	node ->nodeName = "IKNode" + std::to_string(IKidNum);
	IKidNum++;

	return node;
}

bool ikNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	//gef input node and cast to csutome node
	CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, 0));
	if (node)
	{
		//if the node is active
		if (active)
		{
			//set the fix variable to one from variable table
			fix = variable_table->at(nodeName + ".fix").toggle;
			//if the ik node has not been fixed the posion of the effector can change
			if (!fix)
			{
				position = *effector_position_;
			}
			//get the output node of the input
			gef::SkeletonPose pose = node->getOutput();
			//perform ik on pose 
			CalculateCCD(pose, *player_, position, positions);
			//set the output pose to the one out of the ik node
			output_ = pose;
			return true;
		}

	}

	return false;
}


void ikNodeGraph::setup(std::map<std::string, varibaleTable>* table ,  const gef::SkeletonPose* bind, gef::SkinnedMeshInstance* play, gef::Vector4* pos)
{
	//if the node has not been activated set data
	if (!active)
	{
		setBind(bind);
		player_ = play;
		effector_position_ = pos;
		active = true;
		//store variable table and set data
		variable_table = table;

		varibaleTable temp;
		temp.type = dataType::string;
		temp.name = nodeName;
		variable_table->insert({ nodeName, temp });// (animation_name);
		temp.type = dataType::boolean;
		temp.toggle = false;
		variable_table->insert({ nodeName + ".fix", temp });// = float(looping);


	}
}