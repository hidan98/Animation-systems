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
#include <unordered_map>

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

	void cleanUp3d();

	//void graph();
//	ImGui::NodeGraphEditor nge;
    
	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3d_;
	gef::Font* font_;
	gef::InputManager* input_manager_;



	SpriteBasedAnimation* anim;
	SkeletalAnimation2D* bone_;

	void getSpiteFile();




	bool active_graph;
	bool active;

	float fps_;

	nodeGraph* graph;

	void RenderEndEffector();
	void SetCameraMatrices();

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
	bool modelSelected;





	Physics* worldPhysics;

	gef::Vector4* effector_position_;
	float ndc_zmin_;

	PrimitiveRenderer* primitive_renderer_;


	std::map<gef::StringId, varibaleTable> variable_table;
	std::vector<spritePath> sprites;

	bool Active3D;
};

#endif // _SCENE_APP_H
