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

#include "primitive_renderer.h"
#include "gef_debug_drawer.h"

#include "btBulletDynamicsCommon.h"
#include "ragdoll.h"

#include "Physics.h"
#include "primitive_renderer.h"

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
	gef::Renderer3D* renderer_3d_;
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

	void RenderEndEffector();
	void SetCameraMatrices();

	void FrontendInit();
	void FrontendRelease();
	void FrontendUpdate(float frame_time);
	void FrontendRender();

	void SetupLights();
	void SetupCamera();
	gef::Vector4 camera_eye_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	float time;

	bool done;

	gef::Scene* model_scene;
	class gef::Mesh* mesh_;

	gef::Mesh* GetFirstMesh(gef::Scene* scene);
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);

	gef::SkinnedMeshInstance* player_;

	Physics* worldPhysics;

	gef::SkeletonPose ik_pose;
	gef::Vector4* effector_position_;
	float ndc_zmin_;

	PrimitiveRenderer* primitive_renderer_;

	const gef::SkeletonPose* bind_pose;

};

#endif // _SCENE_APP_H
