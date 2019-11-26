#include "Animation_Utils.h"



Animation_Utils::Animation_Utils()
{
}


Animation_Utils::~Animation_Utils()
{
}

gef::Mesh* Animation_Utils::GetFirstMesh(gef::Scene* model_scene, gef::Platform& platform_)
{
	gef::Mesh* mesh = nullptr;

	if (model_scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (model_scene->mesh_data.size() > 0)
			mesh = model_scene->CreateMesh(platform_, model_scene->mesh_data.front());
	}

	return mesh;


}

gef::Skeleton* Animation_Utils::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}