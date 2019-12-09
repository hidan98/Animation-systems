#include "ClipNodeGraph.h"
#include "Animation_Utils.h"


ClipNodeGraph::ClipNodeGraph() : CustomeNode(), clip_(nullptr)
{
	//active = true;
	animTime_ = 0;
	playBackSpeed_ = 1;
	looping = false;
	active = false;
}


ClipNodeGraph::~ClipNodeGraph()
{
	std::map<std::string, varibaleTable>::iterator &it = variable_table->find(animation_name);

	if (it != variable_table->end())
	{
		variable_table->erase(it->first);
		variable_table->erase(animation_name + ".playBackSpeed");
		variable_table->erase(animation_name + ".looping");

	}
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

//void ClipNodeGraph::setup(gef::Platform* plat, const gef::SkeletonPose* bind, std::map<std::string, float>* table_)
//{
//	if (!active)
//	{
//		platform_ = plat;
//		//clip_ = static_cast<gef::Animation*>(clip);
//		
//		setBind(bind);
//		active = true;
//		//bindPose = bind;
//		output_ = *bind;
//		variable_table = table_;
//	}
//	
//}

bool ClipNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{
	


	bool finished = false;
	if (clip_)
	{
		std::string variable_name = this->animation_name + ".playBackSpeed";
		playBackSpeed_ = variable_table->at(variable_name).floatData;

		variable_name = this->animation_name + ".looping";
		looping = variable_table->at(variable_name).toggle;

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
void ClipNodeGraph::setClip(gef::Animation* anim, const gef::SkeletonPose* bind, std::string name)
{
	std::map<std::string, varibaleTable>::iterator &it = variable_table->find(animation_name);

	if (it != variable_table->end())
	{	
		variable_table->erase(it->first);
		variable_table->erase(animation_name + ".playBackSpeed");
		variable_table->erase(animation_name + ".looping");

	}


	animation_name = name;
	varibaleTable temp;
	temp.type = dataType::string;
	temp.name = animation_name;
	variable_table->insert({ animation_name, temp});// (animation_name);
	temp.type = dataType::boolean;
	temp.toggle = false;
	variable_table->insert({ name + ".looping", temp });// = float(looping);
	temp.type = dataType::Real;
	temp.floatData = 1.0f;
	temp.max = 100.f;
	temp.min = 0.0f;
	variable_table->insert({ name + ".playBackSpeed", temp });// = playBackSpeed_;
	clip_ = anim;




}


