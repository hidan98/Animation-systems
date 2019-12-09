#include "ikNodeGraph.h"
#include "ccd.h"


ikNodeGraph::ikNodeGraph() : CustomeNode(), player_(nullptr), effector_position_(nullptr)
{
	position = gef::Vector4(0, 0, 0);
	fix = false;
}


ikNodeGraph::~ikNodeGraph()
{
}

ikNodeGraph* ikNodeGraph::create(const ImVec2& pos)
{
	ikNodeGraph* node = new ikNodeGraph();

	node->nodeName = "IKNode";// +std::to_string(ikNodeID);
	node->init("Ik Node", pos, "in", "out", TYPE);

	node ->nodeName = "IKNode" + std::to_string(IKidNum);
	IKidNum++;

	//ikNodeID++;
	//node->fields.addField(&node->activateRagDoll, "Turn on rag doll");
	//node->activateRagDoll = false;
	//node->fields.add
	return node;
}

bool ikNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{

	CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, 0));
	if (node)
	{
		if (active)
		{
			fix = variable_table->at(nodeName + ".fix").toggle;
			if (!fix)
			{
				position = *effector_position_;
			}
			gef::SkeletonPose pose = node->getOutput();
			//ik_pose = graph->output->getOutput();
			CalculateCCD(pose, *player_, position, positions);

			output_ = pose;
			return true;
		}

	}

	return false;
}


void ikNodeGraph::setup(std::map<std::string, varibaleTable>* table ,  const gef::SkeletonPose* bind, gef::SkinnedMeshInstance* play, gef::Vector4* pos)
{

	if (!active)
	{
		
		setBind(bind);
		player_ = play;
		effector_position_ = pos;
		active = true;



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