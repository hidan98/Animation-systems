#include "ClipNodeGraph.h"
#include "Animation_Utils.h"


ClipNodeGraph::ClipNodeGraph() : CustomeNode(), clip_(nullptr)
{
	float animTime_ = 0;
	bool looping = true;
	float playBackSpeed_ = 1.0f; 
	
}


ClipNodeGraph::~ClipNodeGraph()
{
}

ClipNodeGraph* ClipNodeGraph::create(const ImVec2& pos)
{
	ThisClass* node = (ThisClass*)ImGui::MemAlloc(sizeof(ThisClass)); IM_PLACEMENT_NEW(node) ThisClass();

	// 2) main init
	node->init("Clip node", pos, "", "clip", TYPE);

	
	// 3) init fields ( this uses the node->fields variable; otherwise we should have overridden other virtual methods (to render and serialize) )
	node->fields.addField(&node->speed, 1, "Playback speed", "Speed of the animiation");
	node->speed = 1.0f;
	//clip_ = Animation_Utils::LoadAnimation();


	// 4) set (or load) field values
	//node->Color = ImColor(255, 255, 0, 255);

	return node;
}

bool ClipNodeGraph::process(float dt)
{
	//std::string variable_name = this->animation_name + ".playBackSpeed";
	//playBackSpeed_ = this->tree_->variable_table[variable_name];

	//variable_name = this->animation_name + ".looping";
	//looping = this->tree_->variable_table[variable_name];


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

		output_->SetPoseFromAnim(*clip_, *bindPose, time);
	}
	else
		this->output_ = bindPose;

	return true;
}

bool ClipNodeGraph::update(float dt)
{
	if (clip_) {
		return process(dt);
	}

	else
		return false;

}


