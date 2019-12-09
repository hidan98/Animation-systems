#pragma once
#include"graphics/mesh.h"
#include "graphics/scene.h"
#include "animation/animation.h"

namespace gef {

	class Skeleton;
}

enum class dataType {
	boolean,
	Real,
	intiger,
	string
};

struct varibaleTable
{
	dataType type;
	float max, min;
	float floatData;
	bool toggle;
	std::string name;
};

class Animation_Utils
{
public:
	Animation_Utils();
	~Animation_Utils();

	static gef::Mesh* GetFirstMesh(gef::Scene* model_scene, gef::Platform& platform_);
	static gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	static gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform* platform_);

	static std::vector<std::string> ReadFiles(std::string path);
};

