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


//based on example
static const char* MyNodeTypeNames[MNT_COUNT] = { "IKNode", "Linear Blend","2D Blend", "Rag doll","Clip"
#						ifdef IMGUI_USE_AUTO_BINDING
,"Texture"
#						endif
,"Output"
};

//node factory - based off example for imgu add ons 
static ImGui::Node* MyNodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& nge) {
	switch (nt) {
	case MNT_OUTPUT_NODE: return OutputNodeGraph::create(pos);
	case clipNode: {ClipNodeGraph* temp = ClipNodeGraph::create(pos);
		return temp; }
	case LinearBlendNode: return BlendNodeGraph::create(pos);
	case ragDoll: return RagDollNode::create(pos);
	case ikNode: return ikNodeGraph::create(pos);
	case BledNode2D: return LerpNode2Dgraph::create(pos);
		//case transNode: return TransitionNodeGraph::create(pos);

	default:
		IM_ASSERT(true);    // Missing node type creation
		return NULL;
	}

	return NULL;
}


nodeGraph::nodeGraph(gef::Platform* plat, btDiscreteDynamicsWorld* world) : output(NULL),  platform(nullptr)
{

	platform = plat;
	world_ = world;


}

void nodeGraph::init(gef::Vector4* pos, std::map<gef::StringId, varibaleTable>* table)
{
	effector = pos;
	variabe_table = table;
	std::vector<std::string> modelNames = Animation_Utils::ReadFiles("3DModels");
	//loop through all founds paths
	for (int i = 0; i < modelNames.size(); i++)
	{
		modelData data;
		std::vector<std::string> models = Animation_Utils::ReadFiles(modelNames[i]);
		//loop through all sub files
		for (int j = 0; j < models.size(); j++)
		{
			//save bullet path 
			std::string path = models[j];
			if (path.find(".bullet") != std::string::npos)
			{
				data.bulletPath = path;
			}
			//load animations
			else if (models[j].find("@") != std::string::npos)
			{
				gef::StringId id = gef::GetStringId(path);
				gef::Animation* anim = Animation_Utils::LoadAnimation(path.c_str(), "", platform);
				data.animations_.insert({ id, anim });
				stringTable.insert({ id, path.substr(modelNames[i].size()) });
			}
			//load model file 
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
	
	current.id = pos;
	bind_pose = nullptr;
	nodeGraphEdit = &nge;

	//select the model from the imported model files
	current.model = modelData_[pos].scene_;
		
	//grab new data 
	if (current.mesh_)
	{
		delete current.mesh_;
		current.mesh_ = nullptr;
	}

	current.mesh_= Animation_Utils::GetFirstMesh(current.model, *platform);	
	current.skel = Animation_Utils::GetFirstSkeleton(current.model);
	
	//if a skeliton exhists clean it up
	if (current.skel)
	{
		if (current.skinnedMesh)
		{
			delete current.skinnedMesh;
			current.skinnedMesh = nullptr;

			//clear previous nodes
			nodeGraphEdit->clear();
		}
		current.skinnedMesh = nullptr;

		//set up a new skinned mesh 
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

	//create an output node
	nodeGraphEdit = &nge;
	if (nodeGraphEdit->isInited())
	{

		nodeGraphEdit->registerNodeTypes(MyNodeTypeNames, MNT_COUNT, MyNodeFactory, NULL, -1);

		nodeGraphEdit->registerNodeTypeMaxAllowedInstances(MNT_OUTPUT_NODE, 1);

	}
	//set up outut
	output = static_cast<OutputNodeGraph*>(nodeGraphEdit->addNode(MNT_OUTPUT_NODE, ImVec2(40, 50)));
	output->setBind(bind_pose);

}


nodeGraph::~nodeGraph()
{
}


void nodeGraph::update(float dt)
{	
	ImVector<ImGui::Node*> out;

	//get and store the ouput node every frame 
	nodeGraphEdit->getAllNodesOfType(MNT_OUTPUT_NODE, &out);
	//convert it to our custome type
	CustomeNode* temp = static_cast<CustomeNode*>(out[0]);
	output = temp;
	
}
void nodeGraph::render()
{

	nge;
	nodeGraphEdit = &nge;

	//if the node edditotr has not been set up 
	

	//if we have a model
	if (current.skinnedMesh)
	{
		//grab a node that we have sellected and convert to custome type
		CustomeNode* active = static_cast<CustomeNode*>(nodeGraphEdit->getActiveNode());
		if (active)
		{
			//comparing node types - diffrent interactions and data will need to be sent
			switch (active->getType())
			{
			case clipNode:

				//if a clp node has been selected 
				ImGui::Begin("Animation Selection");	
				
				//loop through all of the animations 
				for (auto it : modelData_[current.id].animations_)
				{
					//create a button and set name 
					if (ImGui::Button(stringTable[it.first].c_str()))
					{
						//if the button has been pressed - cast the node to a clip node set new clip and send data
						ClipNodeGraph* clipGraph = static_cast<ClipNodeGraph*>(active);
						clipGraph->setup(platform, bind_pose, variabe_table);
						clipGraph->setClip(it.second, bind_pose, stringTable[it.first]);

					}
				}

				ImGui::End();
				break;
			case MNT_OUTPUT_NODE:
				//if output node has been selected send data
				active->setup(platform, bind_pose, variabe_table);
				break;
			case ragDoll:
			{
				//if a rag doll node has been selected cast it and send data
				RagDollNode* node = static_cast<RagDollNode*>(active);
				node->setup(variabe_table, bind_pose, world_, modelData_[current.id].bulletPath);
				break;
			}
			case ikNode:
				//if ik node opne imgui window
				ImGui::Begin("bone selection");
				
				//loop through all the joints of the skeliton
				for (int joint_num = 0; joint_num < current.skel->joints().size(); ++joint_num)
				{
					//fins the name in the table 
					std::string bone_name;
					current.model->string_id_table.Find(current.skel->joint(joint_num).name_id, bone_name);

					//create a button with found name
					if (ImGui::Button(bone_name.c_str()))
					{
						//create a pop up window
						ImGui::OpenPopup("pop");
						bonePos = joint_num;
						break;
					}
					
				}
				//if there is a pop up menue
				if (ImGui::BeginPopup("pop"))
				{
					//create a vector for joint indices
					std::vector<int>positions;
					int parent = bonePos;
					//loop through all the parent bones of the selected until we get to the root
					while (parent > -1) {

						//find name and print on button
						std::string beginBone;
						current.model->string_id_table.Find(current.skel->joint(parent).name_id, beginBone);
						//add bone to the temp vector
						positions.push_back(parent);
						if (ImGui::Button(beginBone.c_str()))
						{
							//once we have thae selected bone cast the selected node
							ikNodeGraph* node = static_cast<ikNodeGraph*>(active);
							//reverse the vector so we have them in the right order for the ik
							std::reverse(positions.begin(), positions.end());
							//send data
							node->setPositions(positions);
							node->setup(variabe_table, bind_pose, current.skinnedMesh, effector);
						}
						//if the button was not pressed 
						parent = current.skel->joint(parent).parent;
					}
					ImGui::EndPopup();
				}


				ImGui::End();
				break;

			case LinearBlendNode:
			{
				//if linear blend node selected
				ImGui::Begin("bone selection");

				//used for partal blend 
				std::vector<int> bones;
				static std::vector<bool> temp;

				//loop through all the joints
				for (int i = 0; i < current.skel->joints().size(); i++)
				{
					std::string bone_name;
					current.model->string_id_table.Find(current.skel->joint(i).name_id, bone_name);
					//if the size of the vector is to small add new value and set to false
					int size = temp.size() - 1;
					if (i > (size))
						temp.push_back(false);

					//if we have selected a bone set it to not what it was before
					if (ImGui::Selectable(bone_name.c_str(), temp[i]))
					{
						temp[i] = !temp[i];
	
					}


				}
				//loop through the vector of bools finding only the true values
				for (int i = 0; i < temp.size(); i++)
				{
					if (temp[i])
						bones.push_back(i);
				}

				//cast current node and send data 
				BlendNodeGraph* node = static_cast<BlendNodeGraph*>(active);

				node->setPartalBlends(bones);

				node->setup(variabe_table, bind_pose);
				ImGui::End();
				break;
			}
			case BledNode2D:
			{
				//cast and send data
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
	

	nodeGraphEdit->render();
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

	if (nodeGraphEdit)
	{
		nodeGraphEdit->clear();
	}

	
	
}



