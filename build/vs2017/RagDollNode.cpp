#include "RagDollNode.h"


RagDollNode::RagDollNode() : CustomeNode(), ragdoll_(nullptr), world_(nullptr)
{
	active = false;
}


RagDollNode::~RagDollNode()
{
	//if the table exhists
	if (variable_table)
	{
		if (variable_table->size() > 0)
		{
			//remove data 
			std::map<std::string, varibaleTable>::iterator &it = variable_table->find(ragdollNodeName);

			if (it != variable_table->end())
			{
				variable_table->erase(ragdollNodeName + ".toggle");
				variable_table->erase(it->first);

			}
		}
		
		

	}

	delete ragdoll_;
	ragdoll_ = nullptr;
}


RagDollNode* RagDollNode::create(const ImVec2& pos)
{
	RagDollNode* node = new RagDollNode();

	//set input and output
	node->init("RagDoll node", pos, "in", "out", TYPE);

	//set name and increase count
	node->ragdollNodeName = "RagDoll" + std::to_string(RagDollid);
	RagDollid++;


	return node;
}
bool RagDollNode::process(float dt, ImGui::NodeGraphEditor* editor)
{
	//get input node, if it exhists 
	CustomeNode* node = static_cast<CustomeNode*>(editor->getInputNodeForNodeAndSlot(this, 0));
	if (node)
	{
		//get data 
		activateRagDoll = variable_table->at(ragdollNodeName + ".toggle").toggle;
		//if not activated, update ragdoll based on current pose 
		if (!activateRagDoll)
		{
			ragdoll_->set_pose(node->getOutput());
			ragdoll_->UpdateRagdollFromPose();
			output_ = node->getOutput();
		}
		else
		{
			//let the rag doll do its thing
			ragdoll_->UpdatePoseFromRagdoll();
			output_ = ragdoll_->pose();
		}
		return true;
	}

	return false;
}
void RagDollNode::setup(std::map<std::string, varibaleTable>* table, const gef::SkeletonPose* bind, btDiscreteDynamicsWorld* world, std::string path)
{
	if (!active)
	{
		if (bind->skeleton())
		{
			//set up ragdoll 
			ragdoll_ = new Ragdoll();
			ragdoll_->set_scale_factor(0.01f);

			//set up variable table
			variable_table = table;

			varibaleTable data;
			data.type = dataType::string;
			data.name = ragdollNodeName;
			table->insert({ ragdollNodeName ,data });

			data.type = dataType::boolean;
			data.toggle = false;
			table->insert({ ragdollNodeName + ".toggle", data });

			ragdoll_->Init(*bind, world, path.c_str());
			setBind(bind);
			//SetOutput(bind);
			active = true;
			world_ = world;
		}
		
	}
}

