#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/sprite.h>
#include <input/input_manager.h>
#include "SpriteBasedAnimation.h"
#include "SkeletalAnimation2D.h"
#include <map>
#include "nodeGraph.h"
#include "graphics\skinned_mesh_instance.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
}


class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();

	void UpdateImGuiIO();
	void ImGuiRender();

	//void graph();
//	ImGui::NodeGraphEditor nge;
    
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::InputManager* input_manager_;

	gef::Texture* sprite_texture_;
	gef::Sprite sprite_;

	SpriteBasedAnimation* anim;
	SkeletalAnimation2D* bone_;

	void getSpiteFile();


	std::map<gef::StringId, std::vector<std::string>> sprites;

	std::map<gef::StringId, std::string> lookUp;


	bool active_graph;
	bool active;

	float fps_;

	nodeGraph* graph;

	void FrontendInit();
	void FrontendRelease();
	void FrontendUpdate(float frame_time);
	void FrontendRender();


	float time;



	gef::Scene* model_scene;
	class gef::Mesh* mesh_;

	gef::Mesh* GetFirstMesh(gef::Scene* scene);
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);

	gef::SkinnedMeshInstance* player_;


};

#endif // _SCENE_APP_H
