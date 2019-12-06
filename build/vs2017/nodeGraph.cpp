#include "nodeGraph.h"
#include "Animation_Utils.h"
#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"
#include "BlendNodeGraph.h"
#include <experimental/filesystem>
#include "RagDollNode.h"

nodeGraph::nodeGraph(gef::SkeletonPose pose, gef::Platform* plat, btDiscreteDynamicsWorld* world) : output(NULL),  platform(nullptr)
{
	bind_pose = pose;
	platform = plat;
	setUpAnimations();

	world_ = world;
	//animation = Animation_Utils::LoadAnimation("ybot@dance.scn", "", platform);

	//gef::Scene anim_scene;
	//if (anim_scene.ReadSceneFromFile(*platform, "ybot@jump.scn"))
	//{
	//	// if the animation name is specified then try and find the named anim
	//	// otherwise return the first animation if there is one
	//	std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
	//	if ("")
	//		anim_node_iter = anim_scene.animations.find(gef::GetStringId(""));
	//	else
	//		anim_node_iter = anim_scene.animations.begin();

	//	if (anim_node_iter != anim_scene.animations.end())
	//		animation = new gef::Animation(*anim_node_iter->second);


	//}
}


nodeGraph::~nodeGraph()
{
}

static const char* MyNodeTypeNames[MNT_COUNT] = { "Colour", "Linear Blend", "Rag doll","Clip"
#						ifdef IMGUI_USE_AUTO_BINDING
,"Texture"
#						endif
,"Output"
};


static ImGui::Node* MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& nge) {
	switch (nt) {
	case MNT_OUTPUT_NODE: return OutputNodeGraph::create(pos);
	case clipNode: {ClipNodeGraph* temp = ClipNodeGraph::create(pos);
		//temp->setPlatform()
		
		return temp; }
	case LinearBlendNode: return BlendNodeGraph::create(pos);
	case ragDoll : return RagDollNode::create(pos);
	/*case MNT_OUTPUT_NODE: return OutputNode::Create(pos);*/
#   ifdef IMGUI_USE_AUTO_BINDING
	case MNT_TEXTURE_NODE: return TextureNode::Create(pos);
#   endif //IMGUI_USE_AUTO_BINDING
	default:
		IM_ASSERT(true);    // Missing node type creation
		return NULL;
	}
	
	return NULL;
}

void nodeGraph::setUpAnimations()
{
	namespace fs = std::experimental::filesystem;

	std::vector<std::string> tempStrings;

	for (auto entry : fs::directory_iterator("3DModels"))
	{
		std::vector<animationData*> data;

		std::string temp = entry.path().string();
		for (auto it : fs::directory_iterator(temp))
		{
			std::string tempString = it.path().string();
			std::replace(tempString.begin(), tempString.end(), '\\', '/');
			if (tempString.find(".bullet") != std::string::npos)
			{
				bulletPath = tempString;
			}

			if (tempString.find("@") != std::string::npos)
			{
				animationData* tempData = loadAnimation(tempString);
				std::replace(temp.begin(), temp.end(), '\\', '/');
				
				
				if (tempData)
				{
					tempData->name = tempString.substr(temp.size());
					data.push_back(tempData);
				}
					
			}

			tempStrings.push_back(tempString);
		}
		animations.push_back(data);

	}


}

void nodeGraph::update(float dt)
{

	static ImGui::NodeGraphEditor nge;
	temp_ = &nge;
	if (nge.isInited())
	{

		nge.registerNodeTypes(MyNodeTypeNames, MNT_COUNT, MyNodeFactory, NULL, -1);

		nge.registerNodeTypeMaxAllowedInstances(MNT_OUTPUT_NODE, 1);
		output =  static_cast<OutputNodeGraph*>(nge.addNode(MNT_OUTPUT_NODE, ImVec2(40, 50)));
		output->setBind(&bind_pose);
		/*nge.show_style_editor = true;
		nge.show_load_save_buttons = true;*/
	}
	
	 
	CustomeNode* active = static_cast<CustomeNode*>(nge.getActiveNode());
	if (active)
	{
		switch (active->getType())
		{
		case clipNode:
			ImGui::Begin("Animation Selection");
			//active->setup(platform, &bind_pose, animation);

			for (int i = 0; i < animations[0].size(); i++)
			{

				if (ImGui::Button(animations[0][i]->name.c_str()))
				{
					ClipNodeGraph* clipGraph = static_cast<ClipNodeGraph*>(active);
					clipGraph->setClip(animations[0][i]->animation_, &bind_pose);
				}

			}


			ImGui::End();
			break;
		case MNT_OUTPUT_NODE:
			active->setup(platform, &bind_pose);
			break;
		case ragDoll:
		{
			RagDollNode* node = static_cast<RagDollNode*>(active);
			node->setup(platform, &bind_pose, world_, bulletPath);
			break;
		}
			
		default:
			break;
		}
		
		
	}
	
	ImVector<ImGui::Node*> yes;
	/*nge.getAllNodesOfType(clipNode, &yes);*/


	nge.getAllNodesOfType(MNT_OUTPUT_NODE, &yes);
	CustomeNode* temp = static_cast<CustomeNode*>(yes[0]);
	output = temp;
	//temp->update(dt, temp_);
	
	nge.render();
	
}


animationData* nodeGraph::loadAnimation(std::string path)
{

	gef::Scene* anim_scene = new gef::Scene();
	gef::Animation* animation = nullptr;
	if (anim_scene->ReadSceneFromFile(*platform, path.c_str()))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if ("")
			anim_node_iter = anim_scene->animations.find(gef::GetStringId(""));
		else
			anim_node_iter = anim_scene->animations.begin();

		if (anim_node_iter != anim_scene->animations.end())
		{
			animation = new gef::Animation(*anim_node_iter->second);

			animationData* data = new animationData;
			
			data->animation_ = animation;
			data->scene_ = anim_scene;
			return data;

		}
		else
		{
			delete anim_scene;
			anim_scene = nullptr;
		}
			
	}
	else
	{
		
	}
	return nullptr;
}