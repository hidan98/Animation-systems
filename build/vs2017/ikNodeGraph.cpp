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
			std::map<gef::StringId, varibaleTable>::iterator &it = variable_table->find(nameId);

			if (it != variable_table->end())
			{
				variable_table->erase(it->first);
				variable_table->erase(fixId);
				//variable_table->erase(nodeName + ".looping");

			}
		}

		
	}
}

ikNodeGraph* ikNodeGraph::create(const ImVec2& pos)
{
	ikNodeGraph* node = new ikNodeGraph();

	node->nodeName = "IKNode";// +std::to_string(ikNodeID);
	node->init(node->nodeName.c_str(), pos, "in", "out", TYPE);

	node ->nodeName = "IKNode" + std::to_string(IKidNum);
	node->nameId = gef::GetStringId(node->nodeName);

	node->fixId = gef::GetStringId(node->nodeName + ".fix");
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
			fix = variable_table->at(fixId).toggle;
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


void ikNodeGraph::setup(std::map<gef::StringId, varibaleTable>* table ,  const gef::SkeletonPose* bind, gef::SkinnedMeshInstance* play, gef::Vector4* pos)
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
		variable_table->insert({ nameId, temp });// (animation_name);
		temp.type = dataType::boolean;
		temp.toggle = false;
		temp.name = nodeName + ".fix";
		variable_table->insert({ fixId, temp });// = float(looping);


	}
}

bool ikNodeGraph::render(float)
{
	ImGui::Text("Requires one input\nProvides one output\nUse ImGui window to sellect the bones to be used for ik chain\nUse Variable table to fix bones in place");
	return false;
}