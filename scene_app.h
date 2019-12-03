#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/sprite.h>
#include <input/input_manager.h>
#include "SpriteBasedAnimation.h"
#include "SkeletalAnimation2D.h"
#include <map>


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



	bool active;

	float fps_;

	void FrontendInit();
	void FrontendRelease();
	void FrontendUpdate(float frame_time);
	void FrontendRender();
};

#endif // _SCENE_APP_H
