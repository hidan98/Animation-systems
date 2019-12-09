#include "ClipNodeGraph.h"
#include "Animation_Utils.h"


ClipNodeGraph::ClipNodeGraph() : CustomeNode(), clip_(nullptr)
{
	//active = true;
	animTime_ = 0;
	playBackSpeed_ = 1;
	looping = false;
	//active = false;
}


ClipNodeGraph::~ClipNodeGraph()
{
	if (variable_table)
	{
		std::map<std::string, varibaleTable>::iterator &it = variable_table->find(animation_name);

		if (it != variable_table->end())
		{
			variable_table->erase(it->first);
			variable_table->erase(animation_name + ".playBackSpeed");
			variable_table->erase(animation_name + ".looping");

		}
	}
	
}

ClipNodeGraph* ClipNodeGraph::create(const ImVec2& pos)
{
	ClipNodeGraph* node = new ClipNodeGraph();

	//set up name pos, output and type
	node->init("Clip node", pos, "", "clip", TYPE);


	return node;
}


bool ClipNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{

	bool finished = false;
	//if we have a valid clip
	if (clip_)
	{//grab variables from tabe
		std::string variable_name = this->animation_name + ".playBackSpeed";
		playBackSpeed_ = variable_table->at(variable_name).floatData;

		variable_name = this->animation_name + ".looping";
		looping = variable_table->at(variable_name).toggle;

		//update the time
		animTime_ += dt * playBackSpeed_;
		//if the time is greater than the duration
		if (animTime_ > clip_->duration())
		{
			//reset the time  if looping
			if (looping)
				animTime_ = std::fmodf(animTime_, clip_->duration());
			else
			{
				//set time to duration if not looping
				animTime_ = clip_->duration();
				finished = true;
			}
		}

		float time = animTime_ + clip_->start_time();

		output_.SetPoseFromAnim(*clip_, *bindPose, time);
	}
	else///if we dont have a valid animatino return bind pose
		this->output_ = *bindPose;

	return true;
}

bool ClipNodeGraph::update(float dt, ImGui::NodeGraphEditor* editor)
{
	//no need to not perform this 
	if (active)
	{
		return process(dt, editor);
	}
	else return false;
		


}
void ClipNodeGraph::setClip(gef::Animation* anim, const gef::SkeletonPose* bind, std::string name)
{

	//remove previous animation varables from table
	std::map<std::string, varibaleTable>::iterator &it = variable_table->find(animation_name);

	if (it != variable_table->end())
	{	
		variable_table->erase(it->first);
		variable_table->erase(animation_name + ".playBackSpeed");
		variable_table->erase(animation_name + ".looping");

	}
	//reset variables for table
	
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

	//set the clip
	clip_ = anim;




}


