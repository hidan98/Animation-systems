#pragma once
#include "graphics/skinned_mesh_instance.h"
#include <string>
#include "BlendTree.h"
namespace gef
{
	class Platform;
	class Mesh;
	class Scene;
	class Skeleton;
}

class Animation3D
{
public:
	Animation3D();
	~Animation3D();

	void init(gef::Platform* plat, std::string path);

private:

	gef::Scene* model_scene;
	gef::Platform* platform_;
	gef::Skeleton* skeleton;
	gef::SkinnedMeshInstance* skinned_mesh;
	class gef::Mesh* mesh;

	gef::SkeletonPose bind_pose;
	BlendTree* tree_;
	

};

