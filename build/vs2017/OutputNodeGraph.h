#pragma once
#include "imgui.h"
#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"
#include "CustomeNode.h"

class OutputNodeGraph : public CustomeNode
{
protected:
	typedef Node Base;  //Base Class
	typedef OutputNodeGraph ThisClass;

	OutputNodeGraph();
	~OutputNodeGraph();

	static const int TYPE = MNT_OUTPUT_NODE;

	inline virtual const char* getTooltip() const { return "OutputNode tooltip."; }
	inline virtual const char* getInfo() const { return "OutputNode info.\n\nThis is supposed to display some info about this node."; }
	inline virtual void getDefaultTitleBarColors(ImU32& defaultTitleTextColorOut, ImU32& defaultTitleBgColorOut, float& defaultTitleBgColorGradientOut) const {
		// [Optional Override] customize Node Title Colors [default values: 0,0,-1.f => do not override == use default values from the Style()]
		defaultTitleTextColorOut = IM_COL32(230, 180, 180, 255); defaultTitleBgColorOut = IM_COL32(40, 55, 55, 200); defaultTitleBgColorGradientOut = 0.025f;
	}
	inline virtual bool canBeCopied() const { return false; }

	bool process(float dt, ImGui::NodeGraphEditor* editor);

	bool render(float /*nodeWidth*/);

	bool temp;

	void setup(gef::Platform* plat, const  gef::SkeletonPose* bind);

public:
	static ThisClass* create(const ImVec2& pos);


	// casts:
	inline static ThisClass* Cast(Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	inline static const ThisClass* Cast(const Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
protected:
	

};

