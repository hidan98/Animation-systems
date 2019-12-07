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

	if (active)
	{
		for (int i = 0; i < InputsCount; i++)
		{
			bool input_valid = false;
			CustomeNode* node = static_cast<CustomeNode*>(temp->getInputNodeForNodeAndSlot(this, i));
			if (node)
				input_valid = node->update(dt, temp);

			if (!input_valid)
			{
				output_ = *bindPose;
				all_inputs = false;
				return false;
			}
		}
		bool output_valid = false;

		if (all_inputs)
			output_valid = process(dt, temp);

		return true;
	}
	else
		return false;
	

	
}
