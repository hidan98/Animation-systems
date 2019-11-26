#include "ClipNode.h"
#include "BlendTree.h"


ClipNode::ClipNode(const gef::SkeletonPose* pose, BlendTree* tree, const char* name) : BlendNode(pose, tree), playBackSpeed_(1.0f), animTime_(0), looping(false)
{
	type = "clip node";
	animation_name = name;
	std::string temp = this->animation_name + ".playBackSpeed";
	tree_->variable_table[temp] = 2.0f;

	temp = this->animation_name + ".looping";
	this->tree_->variable_table[temp] = 1;
	
}


ClipNode::~ClipNode()
{
}

bool ClipNode::update(float dt)
{
	if (clip_) {
		return process(dt);
	}
		
	else
		return false;
}

bool ClipNode::process(float dt)
{
	std::string variable_name = this->animation_name + ".playBackSpeed";
	playBackSpeed_ = this->tree_->variable_table[variable_name];

	variable_name = this->animation_name + ".looping";
	looping = this->tree_->variable_table[variable_name];


	bool finished = false;
	if (clip_)
	{
		animTime_ += dt * playBackSpeed_;
		if (animTime_ > clip_->duration())
		{
			if (looping)
				animTime_ = std::fmodf(animTime_, clip_->duration());
			else
			{
				animTime_ = clip_->duration();
				finished = true;
			}
		}
		float time = animTime_ + clip_->start_time();
		
		output_.SetPoseFromAnim(*clip_, *bindPose, time);
	}
	else
		this->output_ = *bindPose;

	return true;
}