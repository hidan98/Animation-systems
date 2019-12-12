#pragma once
#include "CustomeNode.h"
#include "NodeUtilsHeader.h"
#include <unordered_map>
namespace gef {
	class SkeletonPose;
}
static int idCount = 0;
class BlendNodeGraph : public CustomeNode
{
public:
	


protected:

	typedef Node Base;  //Base Class
	typedef BlendNodeGraph ThisClass;
	BlendNodeGraph();
	~BlendNodeGraph();

	static const int TYPE = LinearBlendNode;

	virtual const char* getTooltip() const { return "Clip Node tooltip."; }
	virtual const char* getInfo() const { return "Clip Node info.\n\nThis is supposed to display some info about this node."; }
	/*virtual void getDefaultTitleBarColors(ImU32& defaultTitleTextColorOut, ImU32& defaultTitleBgColorOut, float& defaultTitleBgColorGradientOut) const {
		// [Optional Override] customize Node Title Colors [default values: 0,0,-1.f => do not override == use default values from the Style()]
		defaultTitleTextColorOut = IM_COL32(230, 180, 180, 255); defaultTitleBgColorOut = IM_COL32(40, 55, 55, 200); defaultTitleBgColorGradientOut = 0.025f;
	}*/
	//virtual bool canBeCopied() const { return false; }

	virtual void onEdited() {}
	float speed;

	bool render(float);

	float animTime_;
	bool partalBlend;
	gef::StringId partalToggleId;
	float playBackSpeed_;

	float blendVal;
	gef::StringId blendId;

	std::string Nodename;
	gef::StringId nameId;



	std::vector<int> boneInd;
	//void CustomBlend(const gef::SkeletonPose& start_pose, const gef::SkeletonPose& end_pose);

public:
	static ThisClass* create(const ImVec2& pos);
	//bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);

	// casts:
	inline static ThisClass* Cast(Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	inline static const ThisClass* Cast(const Node* n) { return Node::Cast<ThisClass>(n, TYPE); }

	void setPartalBlends(std::vector<int>& bones) { boneInd.clear(); boneInd = bones; }
	void setup(std::map<gef::StringId, varibaleTable>* table, const gef::SkeletonPose* bind);
};

