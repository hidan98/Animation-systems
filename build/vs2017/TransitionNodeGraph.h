#pragma once
#include "CustomeNode.h"
#include "NodeUtilsHeader.h"
class TransitionNodeGraph : public CustomeNode
{
protected:
	typedef Node Base;  //Base Class
	typedef TransitionNodeGraph ThisClass;

	TransitionNodeGraph();
	~TransitionNodeGraph();


	//static const int TYPE = transNode;

	virtual const char* getTooltip() const { return "Transition Node."; }
	virtual const char* getInfo() const { return "Trans Node info.\n\nThis is supposed to display some info about this node."; }

	float blendAB, blendCD, finalBlend;

	std::string nodeName;
	bool frozen;
	bool smooth;

	float time;

	float timer;
	float setTime;

public:
	static ThisClass* create(const ImVec2& pos);
	//bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);
	void setUp(std::map<std::string, varibaleTable>* tabel, const gef::SkeletonPose* bind);
	// casts:
	//inline static ThisClass* Cast(Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	//inline static const ThisClass* Cast(const Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	
};

