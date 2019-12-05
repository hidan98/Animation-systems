#include "Animation3D.h"
#include "graphics/scene.h"
#include "system/platform.h"
#include "Animation_Utils.h"


Animation3D::Animation3D() : model_scene(nullptr), platform_(nullptr)
{
}


Animation3D::~Animation3D()
{
}

void Animation3D::init(gef::Platform* plat, std::string path)
{
	platform_ = plat;
	model_scene = new gef::Scene();
	model_scene->ReadSceneFromFile(*platform_, path.c_str());
	model_scene->CreateMaterials(*platform_);

	mesh = Animation_Utils::GetFirstMesh(model_scene, *platform_);
	skeleton = Animation_Utils::GetFirstSkeleton(model_scene);

	if (skeleton)
	{
		skinned_mesh = new gef::SkinnedMeshInstance(*skeleton);
		skinned_mesh->set_mesh(mesh);
		
	}


	bind_pose = skinned_mesh->bind_pose();


}