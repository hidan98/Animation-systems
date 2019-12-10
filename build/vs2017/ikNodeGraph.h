#pragma once
#include "CustomeNode.h"
#include "NodeUtilsHeader.h"
static int IKidNum = 0;
class ikNodeGraph : public CustomeNode
{
protected:
	typedef Node Base;  //Base Class
	typedef ikNodeGraph ThisClass;

	ikNodeGraph();
	~ikNodeGraph();

	static const int TYPE = ikNode;
	bool activateRagDoll;

	virtual const char* getTooltip() const { return "Clip Node tooltip."; }
	virtual const char* getInfo() const { return "Clip Node info.\n\nThis is supposed to display some info about this node."; }
	std::vector<int> positions;
	gef::SkinnedMeshInstance* player_;
	gef::Vector4* effector_position_;
	std::string nodeName;
	gef::Vector4 position;
	bool fix;

	gef::StringId nameId;
	gef::StringId fixId;
	
public:

	inline void setPositions(std::vector<int> pos) { positions = pos; }
	static ThisClass* create(const ImVec2& pos);
	//bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);
	static int ikNodeID;
	void setup(std::map<gef::StringId, varibaleTable>* table,const gef::SkeletonPose* bind, gef::SkinnedMeshInstance* play, gef::Vector4* pos);
};

