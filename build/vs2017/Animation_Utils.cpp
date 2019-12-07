#include "Animation_Utils.h"
#include <experimental/filesystem>


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

gef::Animation* Animation_Utils::LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform* platform_)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(*platform_, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(""));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);


	}

	return anim;
}

std::vector<std::string> Animation_Utils::ReadFiles(std::string path)
{
	namespace fs = std::experimental::filesystem;
	std::vector<std::string> files;
	for (auto entry : fs::directory_iterator(path))
	{
		std::string output = entry.path().string();
		std::replace(output.begin(), output.end(), '\\', '/');
		files.push_back(output);
	}
	return files;

}
