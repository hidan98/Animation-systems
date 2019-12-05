#pragma once
#include "imgui.h"
#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"

#include "ClipNode.h"
class ClipNodeGraph : public ImGui::Node
{
protected:

	typedef Node Base;  //Base Class
	typedef ClipNodeGraph ThisClass;
	ClipNodeGraph();
	~ClipNodeGraph();

	static const int TYPE = clipNode;

	virtual const char* getTooltip() const { return "Clip Node tooltip."; }
	virtual const char* getInfo() const { return "Clip Node info.\n\nThis is supposed to display some info about this node."; }
	/*virtual void getDefaultTitleBarColors(ImU32& defaultTitleTextColorOut, ImU32& defaultTitleBgColorOut, float& defaultTitleBgColorGradientOut) const {
		// [Optional Override] customize Node Title Colors [default values: 0,0,-1.f => do not override == use default values from the Style()]
		defaultTitleTextColorOut = IM_COL32(230, 180, 180, 255); defaultTitleBgColorOut = IM_COL32(40, 55, 55, 200); defaultTitleBgColorGradientOut = 0.025f;
	}*/
	//virtual bool canBeCopied() const { return false; }


	float speed;

	ClipNode* node_;

public:
	static ThisClass* create(const ImVec2& pos);
	void update();

	// casts:
	inline static ThisClass* Cast(Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	inline static const ThisClass* Cast(const Node* n) { return Node::Cast<ThisClass>(n, TYPE); }

	
};

