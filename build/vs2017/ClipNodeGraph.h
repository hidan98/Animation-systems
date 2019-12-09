#pragma once
#include "imgui.h"
#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"
#include <animation/animation.h>
#include "ClipNode.h"
#include"CustomeNode.h"
class ClipNodeGraph : public CustomeNode
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

	virtual void onEdited() {}
	float speed;

	ClipNode* node_;

	gef::Animation* clip_;

	float animTime_;
	bool looping;
	float playBackSpeed_;

	bool active;

	void setup(gef::Platform* plat, const gef::SkeletonPose* bind, std::map<std::string, float>* table);
	std::string animation_name;
	
	//void onEditField(ImGui::FieldInfo& /*f*/, int widgetIndex) {
	//	//fprintf(stderr,"TextureNode::onEditField(\"%s\",%i);\n",f.label,widgetIndex);
	//	if (widgetIndex == 1)         startBrowseDialogNextFrame = true;  // browsing button pressed
	//	else if (widgetIndex == 0)    processPath(imagePath);             // text edited (= "return" pressed in our case)


	//}

	//static void StaticEditFieldCallback(ImGui::FieldInfo& f, int widgetIndex) {
	//	reinterpret_cast<ThisClass*>(f.userData)->onEditField(f, widgetIndex);
	//}

	


public:
	static ThisClass* create(const ImVec2& pos);
	bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);
	inline void setClip(gef::Animation* anim, const gef::SkeletonPose* bind, std::string name);
	// casts:
	inline static ThisClass* Cast(Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	inline static const ThisClass* Cast(const Node* n) { return Node::Cast<ThisClass>(n, TYPE); }

	
};

