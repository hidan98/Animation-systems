#pragma once
#include "imgui.h"
#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"
#include <animation/animation.h>
#include "ClipNode.h"
#include"CustomeNode.h"

static int clipNodeID = 0;
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


	virtual void onEdited() {}
	float speed;

	gef::Animation* clip_;
	std::string nodeName;
	float animTime_;
	bool looping;
	float playBackSpeed_;

	//bool active;

	//void setup(gef::Platform* plat, const gef::SkeletonPose* bind, std::map<std::string, float>* table);
	std::string animation_name;
	

public:
	static ThisClass* create(const ImVec2& pos);
	bool update(float dt, ImGui::NodeGraphEditor* editor);
	bool process(float dt, ImGui::NodeGraphEditor* editor);
	void setClip(gef::Animation* anim, const gef::SkeletonPose* bind, std::string name);
	// casts:
	inline static ThisClass* Cast(Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	inline static const ThisClass* Cast(const Node* n) { return Node::Cast<ThisClass>(n, TYPE); }
	inline float getTime() { return animTime_; }
	inline float getDuration() { return clip_->duration(); }	
	
};

