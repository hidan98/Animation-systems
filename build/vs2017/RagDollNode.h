#pragma once
#include"CustomeNode.h"
#include "NodeUtilsHeader.h"
#include "ragdoll.h"
class RagDollNode: public CustomeNode
{

protected:
	typedef Node Base;  //Base Class
	typedef RagDollNode ThisClass;

	RagDollNode();
	~RagDollNode();

	static const int TYPE = ragDoll;
	bool activateRagDoll;

	virtual const char* getTooltip() const { return "Clip Node tooltip."; }
	virtual const char* getInfo() const { return "Clip Node info.\n\nThis is supposed to display some info about this node."; }
	Ragdoll* ragdoll_;


public:
	static ThisClass* create(const ImVec2& pos);
	//bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);
	void setup(gef::Platform* plat, gef::SkeletonPose* bind, void* data);
	void setup(gef::Platform* plat, gef::SkeletonPose* bind, btDiscreteDynamicsWorld* world, std::string path);

};
