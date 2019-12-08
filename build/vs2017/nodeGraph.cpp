#include "nodeGraph.h"
#include "Animation_Utils.h"
#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"
#include "BlendNodeGraph.h"
#include <experimental/filesystem>
#include "RagDollNode.h"
#include "ikNodeGraph.h"


nodeGraph::nodeGraph(const gef::SkeletonPose* pose, gef::Platform* plat, btDiscreteDynamicsWorld* world, gef::SkinnedMeshInstance* mesh) : output(NULL),  platform(nullptr), skinned_mesh_player(nullptr)
{
	bind_pose = pose;
	platform = plat;
	//setUpAnimations();

	world_ = world;

	skinned_mesh_player = mesh;
}

void nodeGraph::init(gef::Vector4* pos)
{
	effector = pos;

	std::vector<std::string> temp = Animation_Utils::ReadFiles("3DModels");
	for (int i = 0; i < temp.size(); i++)
	{
		modelData* data = new modelData;
		std::vector<std::string> models = Animation_Utils::ReadFiles(temp[i]);

		for (int j = 0; j < models.size(); j++)
		{
			std::string path = models[j];
			if (path.find(".bullet") != std::string::npos)
			{
				data->bulletPath = path;
			}
			else if (models[j].find("@") != std::string::npos)
			{
				gef::StringId id = gef::GetStringId(path);
				gef::Animation* anim = Animation_Utils::LoadAnimation(path.c_str(), "", platform);
				data->animations_.insert({ id, anim });
				stringTable.insert({ id, path.substr(temp[i].size()) });
			}
			else if (path.find(".scn") != std::string::npos)
			{
				data->scene_ = new gef::Scene();
				data->scene_->ReadSceneFromFile(*platform, path.c_str());

			}
		}
		modelData_.push_back(data);
	}


}


nodeGraph::~nodeGraph()
{
}

static const char* MyNodeTypeNames[MNT_COUNT] = { "IKNode", "Linear Blend", "Rag doll","Clip"
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
	case ikNode: return ikNodeGraph::create(pos);
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


void nodeGraph::update(float dt)
{
	//ImGui::BeginMenuBar()
	static ImGui::NodeGraphEditor nge;
	temp_ = &nge;
	if (nge.isInited())
	{

		nge.registerNodeTypes(MyNodeTypeNames, MNT_COUNT, MyNodeFactory, NULL, -1);

		nge.registerNodeTypeMaxAllowedInstances(MNT_OUTPUT_NODE, 1);
		output =  static_cast<OutputNodeGraph*>(nge.addNode(MNT_OUTPUT_NODE, ImVec2(40, 50)));
		output->setBind(bind_pose);
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

			
			for(auto it : modelData_[0]->animations_)
			{
				if (ImGui::Button(stringTable[it.first].c_str()))
				{
					ClipNodeGraph* clipGraph = static_cast<ClipNodeGraph*>(active);
					clipGraph->setClip(it.second, bind_pose);
				}
			}

			ImGui::End();
			break;
		case MNT_OUTPUT_NODE:
			active->setup(platform, bind_pose);
			break;
		case ragDoll:
		{
			RagDollNode* node = static_cast<RagDollNode*>(active);
			node->setup(platform, bind_pose, world_, modelData_[0]->bulletPath);
			break;
		}
		case ikNode:
			
			ImGui::Begin("bone selection");
			

			for (int joint_num = 0; joint_num < current.skel->joints().size(); ++joint_num)
			{
				std::string bone_name;
				current.model->string_id_table.Find(current.skel->joint(joint_num).name_id, bone_name);
				if (ImGui::Button(bone_name.c_str()))
				{
					ImGui::OpenPopup("pop");
					bonePos = joint_num;
					break;
				}			
				//model_scene_->string_id_table.Find(skeleton->joint(joint_num).name_id, bone_name);
				//gef::DebugOut("%d: %s\n", joint_num, bone_name.c_str());
			}
			if (ImGui::BeginPopup("pop"))
			{
				std::vector<int>positions;
				int parent = bonePos;
				while (parent > -1) {

					std::string beginBone;
					current.model->string_id_table.Find(current.skel->joint(parent).name_id, beginBone);
					positions.push_back(parent);
					if (ImGui::Button(beginBone.c_str()))
					{
						ikNodeGraph* node = static_cast<ikNodeGraph*>(active);
						std::reverse(positions.begin(), positions.end());
						node->setPositions(positions);
						node->setup(platform, bind_pose, skinned_mesh_player, effector);
					}
					parent = current.skel->joint(parent).parent;
				}
				ImGui::EndPopup();
			}


			ImGui::End();
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




