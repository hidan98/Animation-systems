#pragma once
#include"CustomeNode.h"
#include "NodeUtilsHeader.h"
#include "ragdoll.h"
#include "btBulletDynamicsCommon.h"

static int RagDollid = 0;
class RagDollNode: public CustomeNode
{

protected:
	typedef Node Base;  //Base Class
	typedef RagDollNode ThisClass;

	RagDollNode();
	~RagDollNode();

	static const int TYPE = ragDoll;
	bool activateRagDoll;

	inline virtual const char* getTooltip() const { return "Clip Node tooltip."; }
	inline virtual const char* getInfo() const { return "Clip Node info.\n\nThis is supposed to display some info about this node."; }
	Ragdoll* ragdoll_;
	btDynamicsWorld* world_;

	std::string ragdollNodeName;

	gef::StringId nameID;
	gef::StringId toggleID;

	bool render(float);


public:
	static ThisClass* create(const ImVec2& pos);
	//bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);
	void setup(std::map<gef::StringId, varibaleTable>* table, const gef::SkeletonPose* bind, btDiscreteDynamicsWorld* world, std::string path);

};

