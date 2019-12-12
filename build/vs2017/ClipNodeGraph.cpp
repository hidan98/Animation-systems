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
		if (variable_table->size() > 0)
		{
			std::map<gef::StringId, varibaleTable>::iterator &it = variable_table->find(nameId);

			if (it != variable_table->end())
			{
				variable_table->erase(it->first);
				variable_table->erase(playBackID);
				variable_table->erase(loopingId);

			}
		}
		
	}
	
}

ClipNodeGraph* ClipNodeGraph::create(const ImVec2& pos)
{
	ClipNodeGraph* node = new ClipNodeGraph();

	//set up name pos, output and type
	

	node->nodeName = "ClipNode" + std::to_string(clipNodeID);
	node->init(node->nodeName.c_str(), pos, "", "clip", TYPE);

	node->nameId = gef::GetStringId(node->nodeName);

	node->playBackID = gef::GetStringId(node->nodeName + ".playbackSpeed");
	node->loopingId = gef::GetStringId(node->nodeName + ".looping");
	clipNodeID++;


	return node;
}


bool ClipNodeGraph::process(float dt, ImGui::NodeGraphEditor* editor)
{

	bool finished = false;
	//if we have a valid clip
	if (clip_)
	{//grab variables from tabe
		//std::string variable_name = this->nodeName + animation_name + ".playBackSpeed";
		playBackSpeed_ = variable_table->at(playBackID).floatData;

		//variable_name = this-> nodeName + animation_name + ".looping";
		looping = variable_table->at(loopingId).toggle;

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
	std::map<gef::StringId, varibaleTable>::iterator &it = variable_table->find(nameId);

	if (it != variable_table->end())
	{	
		variable_table->erase(it->first);
		variable_table->erase(playBackID);
		variable_table->erase(loopingId);

	}
	//reset variables for table
	
	animation_name = name;
	varibaleTable temp;
	temp.type = dataType::string;
	temp.name = nodeName + animation_name;
	nameId = gef::GetStringId(nodeName + animation_name);
	variable_table->insert({ nameId, temp});// (animation_name);


	temp.type = dataType::boolean;
	temp.toggle = false;
	loopingId = gef::GetStringId(nodeName + animation_name + ".looping");
	
	temp.name = nodeName + animation_name + ".looping";
	variable_table->insert({ loopingId, temp });// = float(looping);
	temp.type = dataType::Real;
	temp.floatData = 1.0f;
	temp.max = 100.f;
	temp.min = 0.0f;

	temp.name = nodeName + name + ".playBackSpeed";
	playBackID = gef::GetStringId(nodeName + name + ".playBackSpeed");

	variable_table->insert({ playBackID, temp });// = playBackSpeed_;

	//set the clip
	clip_ = anim;




}


bool ClipNodeGraph::render(float)
{

	ImGui::Text("Provides one output\nUse ImGui window to sellect aniation\nUse Variable table to edit playback");
	return false;
}

