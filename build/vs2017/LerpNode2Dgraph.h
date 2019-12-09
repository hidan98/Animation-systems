#pragma once
#include "CustomeNode.h"
#include "NodeUtilsHeader.h"

static int NodeId2D = 0;

class LerpNode2Dgraph : public CustomeNode
{
protected:
	typedef Node Base;  //Base Class
	typedef LerpNode2Dgraph ThisClass;

	LerpNode2Dgraph();
	~LerpNode2Dgraph();


	static const int TYPE = BledNode2D;

	virtual const char* getTooltip() const { return "Clip Node tooltip."; }
	virtual const char* getInfo() const { return "Clip Node info.\n\nThis is supposed to display some info about this node."; }

	float blendAB, blendCD, finalBlend;

	std::string nodeName;

public:
	static ThisClass* create(const ImVec2& pos);
	//bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);
	void setUp(std::map<std::string, varibaleTable>* tabel, const gef::SkeletonPose* bind);
	// casts:
	inline static ThisClass* Cast(Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	inline static const ThisClass* Cast(const Node* n) { return Node::Cast<ThisClass>(n, TYPE); }

};

