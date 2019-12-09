#include "CustomeNode.h"
#include "imguinodegrapheditor.h"


CustomeNode::CustomeNode() : ImGui::Node(), bindPose(nullptr)
{
	active = false;
}


CustomeNode::~CustomeNode()
{
}

bool CustomeNode::update(float dt, ImGui::NodeGraphEditor* temp)
{
	bool all_inputs = true;
	//ImGui::NodeGraphEditor editor = getNodeGraphEditor();
	int amount = InputsCount;

	//if the node has been activated
	if (active)
	{
		//loop through all inputs
		for (int i = 0; i < InputsCount; i++)
		{
			//garb input node and cast
			bool input_valid = false;
			CustomeNode* node = static_cast<CustomeNode*>(temp->getInputNodeForNodeAndSlot(this, i));
			//if the node is valid udate and get responce 
			if (node)
				input_valid = node->update(dt, temp);

			//if the node update return false set ouput to bind and return false
			if (!input_valid)
			{
				output_ = *bindPose;
				all_inputs = false;
				return false;
			}
		}
		bool output_valid = false;

		//if all inputs were vailid process itself
		if (all_inputs)
			return output_valid = process(dt, temp);

		//return true;
	}
	else
		return false;
	

	
}
