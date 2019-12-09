#include "nodeGraph.h"
#include "Animation_Utils.h"
#include "imguinodegrapheditor.h"
#include "NodeUtilsHeader.h"
#include "BlendNodeGraph.h"
#include <experimental/filesystem>
#include "RagDollNode.h"
#include "ikNodeGraph.h"
#include "LerpNode2Dgraph.h"
#include <unordered_map>
#include "TransitionNodeGraph.h"

#
static const char* MyNodeTypeNames[MNT_COUNT] = { "IKNode", "Linear Blend","2D Blend", "Rag doll","Clip"
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
	case ragDoll: return RagDollNode::create(pos);
	case ikNode: return ikNodeGraph::create(pos);
	case BledNode2D: return LerpNode2Dgraph::create(pos);
		//case transNode: return TransitionNodeGraph::create(pos);
#   ifdef IMGUI_USE_AUTO_BINDING
	case MNT_TEXTURE_NODE: return TextureNode::Create(pos);
#   endif //IMGUI_USE_AUTO_BINDING
	default:
		IM_ASSERT(true);    // Missing node type creation
		return NULL;
	}

	return NULL;
}


nodeGraph::nodeGraph(gef::Platform* plat, btDiscreteDynamicsWorld* world) : output(NULL),  platform(nullptr)
{

	platform = plat;
	//setUpAnimations();

	world_ = world;


}

void nodeGraph::init(gef::Vector4* pos, std::map<std::string, varibaleTable>* table)
{
	effector = pos;
	variabe_table = table;
	modelNames = Animation_Utils::ReadFiles("3DModels");
	for (int i = 0; i < modelNames.size(); i++)
	{
		modelData data;
		std::vector<std::string> models = Animation_Utils::ReadFiles(modelNames[i]);

		for (int j = 0; j < models.size(); j++)
		{
			std::string path = models[j];
			if (path.find(".bullet") != std::string::npos)
			{
				data.bulletPath = path;
			}
			else if (models[j].find("@") != std::string::npos)
			{
				gef::StringId id = gef::GetStringId(path);
				gef::Animation* anim = Animation_Utils::LoadAnimation(path.c_str(), "", platform);
				data.animations_.insert({ id, anim });
				stringTable.insert({ id, path.substr(modelNames[i].size()) });
			}
			else if (path.find(".scn") != std::string::npos)
			{
				data.scene_ = new gef::Scene();
				data.scene_->ReadSceneFromFile(*platform, path.c_str());
				data.scene_->CreateMaterials(*platform);

			}
		}
		modelData_.push_back(data);
	}


}
static ImGui::NodeGraphEditor nge;

void nodeGraph::setUpModel(int pos)
{
	
	bind_pose = nullptr;
	nodeGraphEdit = &nge;

	current.model = modelData_[pos].scene_;
	
	//nodeGraphEdit->clear();
		
	current.mesh_= Animation_Utils::GetFirstMesh(current.model, *platform);

	
	current.skel = Animation_Utils::GetFirstSkeleton(current.model);
	
	if (current.skel)
	{
		if (current.skinnedMesh)
		{
			delete current.skinnedMesh;
			current.skinnedMesh = nullptr;
			nodeGraphEdit->clear();
		}
		current.skinnedMesh = nullptr;
		
		current.skinnedMesh = new gef::SkinnedMeshInstance(*current.skel);
		current.skinnedMesh->set_mesh(current.mesh_);
		current.skinnedMesh->UpdateBoneMatrices(current.skinnedMesh->bind_pose());
		bind_pose = &current.skinnedMesh->bind_pose();
	}

	if (output)
	{
		//delete output;
		output = nullptr;
	}

	//static ImGui::NodeGraphEditor nge;
	nodeGraphEdit = &nge;
	if (nodeGraphEdit->isInited())
	{

		nge.registerNodeTypes(MyNodeTypeNames, MNT_COUNT, MyNodeFactory, NULL, -1);

		nge.registerNodeTypeMaxAllowedInstances(MNT_OUTPUT_NODE, 1);

	}
	output = static_cast<OutputNodeGraph*>(nodeGraphEdit->addNode(MNT_OUTPUT_NODE, ImVec2(40, 50)));
	output->setBind(bind_pose);

}


nodeGraph::~nodeGraph()
{
}


void nodeGraph::update(float dt)
{	
	 
	
	
	ImVector<ImGui::Node*> out;


	nodeGraphEdit->getAllNodesOfType(MNT_OUTPUT_NODE, &out);
	CustomeNode* temp = static_cast<CustomeNode*>(out[0]);
	output = temp;
	
}
void nodeGraph::render()
{

	nge;
	nodeGraphEdit = &nge;

	if (nodeGraphEdit->isInited())
	{

		nge.registerNodeTypes(MyNodeTypeNames, MNT_COUNT, MyNodeFactory, NULL, -1);

		nge.registerNodeTypeMaxAllowedInstances(MNT_OUTPUT_NODE, 1);
	}

	if (current.skinnedMesh)
	{
		CustomeNode* active = static_cast<CustomeNode*>(nodeGraphEdit->getActiveNode());
		if (active)
		{
			switch (active->getType())
			{
			case clipNode:
				ImGui::Begin("Animation Selection");
				//active->setup(platform, &bind_pose, animation);


				for (auto it : modelData_[0].animations_)
				{
					if (ImGui::Button(stringTable[it.first].c_str()))
					{
						ClipNodeGraph* clipGraph = static_cast<ClipNodeGraph*>(active);
						clipGraph->setup(platform, bind_pose, variabe_table);
						clipGraph->setClip(it.second, bind_pose, stringTable[it.first]);

					}
				}

				ImGui::End();
				break;
			case MNT_OUTPUT_NODE:
				active->setup(platform, bind_pose, variabe_table);
				break;
			case ragDoll:
			{
				RagDollNode* node = static_cast<RagDollNode*>(active);
				node->setup(platform, bind_pose, world_, modelData_[0].bulletPath);
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
							node->setup(variabe_table, bind_pose, current.skinnedMesh, effector);
						}
						parent = current.skel->joint(parent).parent;
					}
					ImGui::EndPopup();
				}


				ImGui::End();
				break;

			case LinearBlendNode:
			{


				ImGui::Begin("bone selection");

				std::vector<int> bones;
				static std::vector<bool> temp;

				for (int i = 0; i < current.skel->joints().size(); i++)
				{
					std::string bone_name;
					current.model->string_id_table.Find(current.skel->joint(i).name_id, bone_name);

					int size = temp.size() - 1;
					if (i > (size))
						temp.push_back(false);


					if (ImGui::Selectable(bone_name.c_str(), temp[i]))
					{
						temp[i] = !temp[i];
						//size = 100;
					}


				}

				for (int i = 0; i < temp.size(); i++)
				{
					if (temp[i])
						bones.push_back(i);
				}

				BlendNodeGraph* node = static_cast<BlendNodeGraph*>(active);

				node->setPartalBlends(bones);

				node->setup(variabe_table, bind_pose);
				ImGui::End();
				break;
			}
			case BledNode2D:
			{
				LerpNode2Dgraph* node = static_cast<LerpNode2Dgraph*>(active);
				node->setUp(variabe_table, bind_pose);
				break;
			}

			default:
				active->setup(platform, bind_pose, variabe_table);
				break;
			}


		}
	}
	

	nge.render();
}

void nodeGraph::cleanup()
{

	bind_pose = nullptr;
	platform = nullptr;
	//setUpAnimations();

	world_ = nullptr;


	variabe_table = nullptr;

	effector = nullptr;
	current_model = nullptr;


	for (int i = 0; i < modelData_.size(); i++)
	{
		delete modelData_[i].scene_;
		modelData_[i].scene_ = nullptr;

		for (auto &it : modelData_[i].animations_)
		{
			delete it.second;
			it.second = nullptr;			
		}
	}

	if (current.skinnedMesh)
	{
		delete current.skinnedMesh;
		current.skinnedMesh = nullptr;
	}
	

	if (current.mesh_)
	{
		delete current.mesh_;
		current.mesh_ = nullptr;
	}

	
	
}



