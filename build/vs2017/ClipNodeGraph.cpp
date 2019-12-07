#include "ClipNodeGraph.h"
#include "Animation_Utils.h"


ClipNodeGraph::ClipNodeGraph() : CustomeNode(), clip_(nullptr)
{
	//active = true;
	animTime_ = 0;
	animTime_ = 0;
	playBackSpeed_ = 1;
	looping = true;
	active = false;
}


ClipNodeGraph::~ClipNodeGraph()
{
}

ClipNodeGraph* ClipNodeGraph::create(const ImVec2& pos)
{
	ThisClass* node = (ThisClass*)ImGui::MemAlloc(sizeof(ThisClass)); IM_PLACEMENT_NEW(node) ThisClass();

	// 2) main init
	node->init("Clip node", pos, "", "clip", TYPE);

	ImGui::FieldInfo* f = NULL;

	//f = &node->fields.addFieldCustom();
	// 3) init fields ( this uses the node->fields variable; otherwise we should have overridden other virtual methods (to render and serialize) )
	node->fields.addField(&node->speed, 1, "Playback speed", "Speed of the animiation");
	node->fields.addField(&node->active, "edit", "pop up active");
	//node->fields.addField
	node->speed = 1.0f;
	//clip_ = Animation_Utils::LoadAnimation();
	//f->editedFieldDelegate = &ThisClass::StaticEditFieldCallback;/*

	// 4) set (or load) field values
	//node->Color = ImColor(255, 255, 0, 255);

	return node;
}

void ClipNodeGraph::setup(gef::Platform* plat, const gef::SkeletonPose* bind, void* clip)
{
	if (!active)
	{
		platform_ = plat;
		clip_ = static_cast<gef::Animation*>(clip);
		
		setBind(bind);
		active = true;
		//bindPose = bind;
		output_ = *bind;
	}
	
}

bool ClipNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
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

		output_.SetPoseFromAnim(*clip_, *bindPose, time);
	}
	else
		this->output_ = *bindPose;

	return true;
}

bool ClipNodeGraph::update(float dt, ImGui::NodeGraphEditor* editor)
{
	if (clip_) {
		return process(dt, editor);
	}

	else
		return false;

}


