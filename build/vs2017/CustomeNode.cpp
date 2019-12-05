#include "CustomeNode.h"
#include "imguinodegrapheditor.h"


CustomeNode::CustomeNode() : ImGui::Node()
{
	
}


CustomeNode::~CustomeNode()
{
}

bool CustomeNode::update(float dt)
{
	bool all_inputs = true;
	ImGui::NodeGraphEditor editor = getNodeGraphEditor();
	int amount = InputsCount;


	for (int i = 0; i < InputsCount; i++)
	{
		bool input_valid = false;
		CustomeNode* node = static_cast<CustomeNode*>( editor.getInputNodeForNodeAndSlot(this, i));
		if (node)
			input_valid = node->update(dt);

		if (!input_valid)
		{
			all_inputs = false;
			return false;
		}
	}

	bool output_valid = false;

	if (all_inputs)
		output_valid = process(dt);

	return true;
}
