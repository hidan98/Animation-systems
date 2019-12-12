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

	inline virtual const char* getTooltip() const { return "Clip Node tooltip."; }
	inline virtual const char* getInfo() const { return "Clip Node info.\n\nThis is supposed to display some info about this node."; }


	//virtual void onEdited() {}
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

	inline void setPartalBlends(std::vector<int>& bones) { boneInd.clear(); boneInd = bones; }
	void setup(std::map<gef::StringId, varibaleTable>* table, const gef::SkeletonPose* bind);
};

