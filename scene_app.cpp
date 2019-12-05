#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <input/touch_input_manager.h>
#include <graphics/sprite.h>
#include "load_texture.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Application.h"
#include <platform/d3d11/system/platform_d3d11.h>
#include <platform\d3d11\input\keyboard_d3d11.h>

#include <experimental\filesystem>
#include <vector>
#include <string>

ImTextureID Application_LoadTexture(const char* path)
{
	return ImGui_LoadTexture(path);
}

ImTextureID Application_CreateTexture(const void* data, int width, int height)
{
	return ImGui_CreateTexture(data, width, height);
}

void Application_DestroyTexture(ImTextureID texture)
{
	ImGui_DestroyTexture(texture);
}

int Application_GetTextureWidth(ImTextureID texture)
{
	return ImGui_GetTextureWidth(texture);
}

int Application_GetTextureHeight(ImTextureID texture)
{
	return ImGui_GetTextureHeight(texture);
}

// Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	input_manager_(NULL),
	font_(NULL),
	sprite_texture_(NULL)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	InitFont();

	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	sprite_texture_ = CreateTextureFromPNG("Idle (1).png", platform_);

	sprite_.set_texture(sprite_texture_);
	sprite_.set_position(gef::Vector4(platform_.width()*0.5f, platform_.height()*0.5f, -0.99f));
	sprite_.set_height(128.0f);
	sprite_.set_width(128.0f);




	anim = new SpriteBasedAnimation();
	anim->init("boy-attack_tex.json", "boy-attack_ske.json", "boy-attack_tex.png", platform_);

	bone_ = new SkeletalAnimation2D();
	bone_->init("Dragon_tex.json", "Dragon_ske.json", "Dragon_tex.png", platform_);

	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	InitFont();

	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	active = false;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	gef::PlatformD3D11& platform_d3d = static_cast<gef::PlatformD3D11&>(platform_);

	ImGui_ImplWin32_Init(platform_d3d.hwnd());
	ImGui_ImplDX11_Init(platform_d3d.device(), platform_d3d.device_context());

	//Application_Initialize();
	getSpiteFile();
}

void SceneApp::CleanUp()
{
	//Application_Finalize();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	delete sprite_texture_;
	sprite_texture_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	CleanUpFont();

	delete sprite_renderer_;
	sprite_renderer_ = NULL;
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;


	input_manager_->Update();
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = input_manager_->touch_manager()->is_button_down(0);
		io.MouseDown[1] = input_manager_->touch_manager()->is_button_down(1);
		io.MouseWheel += input_manager_->touch_manager()->mouse_rel().z() / WHEEL_DELTA;

		gef::KeyboardD3D11* keyboard_d3d11 = (gef::KeyboardD3D11*)input_manager_->keyboard();


		io.KeyShift = keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_LSHIFT) || keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_RSHIFT);
		io.KeyCtrl = keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_LCONTROL) || keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_RCONTROL);
		io.KeyAlt = keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_LALT) || keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_RALT);


		for (int key_code_num = 0; key_code_num < gef::Keyboard::NUM_KEY_CODES; ++key_code_num)
		{
			gef::Keyboard::KeyCode key_code = (gef::Keyboard::KeyCode)key_code_num;
			int scan_code = keyboard_d3d11->GetScanCode(key_code);

			int vk_code = MapVirtualKey(scan_code, MAPVK_VSC_TO_VK_EX);

			io.KeysDown[vk_code] = keyboard_d3d11->IsKeyDown(key_code);
			if (keyboard_d3d11->IsKeyPrintable(key_code) && keyboard_d3d11->IsKeyPressed(key_code))
			{
				if (!io.KeyShift && vk_code >= 'A' && vk_code <= 'Z')
					vk_code = 'a' + (vk_code - 'A');
				io.AddInputCharacter(vk_code);
			}
		}
	}

	anim->update(frame_time, gef::Vector2(platform_.width()*0.5f, platform_.height()*0.5f));

	bone_->update(frame_time, gef::Vector2(platform_.width()*0.5f, platform_.height()*0.5f));

	return true;
}




void SceneApp::Render()
{
	sprite_renderer_->Begin();

	// Render button icon
	sprite_renderer_->DrawSprite(*anim->getSprite());
	if(active)
		bone_->render(sprite_renderer_);

	DrawHUD();
	sprite_renderer_->End();

	ImGuiRender();
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}


void SceneApp::ImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO();

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("2D"))
		{
			if (ImGui::BeginMenu("Sprite based animation"))
			{
				for (auto it : sprites)
				{
					if (ImGui::MenuItem(lookUp[it.first].c_str()))
					{
						std::string png, tex, ske;
						for (int i = 0; i < it.second.size(); i++)
						{
							if (it.second[i].find(".png") != std::string::npos)
							{
								png = it.second[i];
							}
							else if (it.second[i].find("ske.json") != std::string::npos)
							{
								ske = it.second[i];
							}
							else if (it.second[i].find("tex.json") != std::string::npos)
							{
								tex = it.second[i];
							}
						}
						anim->cleanUp();

						anim->init(tex.c_str(), ske.c_str(), png.c_str(), platform());
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Bone Animation"))
			{
				for (int i = 0; i < bone_->getData()->bone_armiture->animation.size(); i++)
				{
					if (ImGui::MenuItem(bone_->getData()->bone_armiture->animation[i]->name.c_str()))
					{
						bone_->setAnimationNum(i);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}









		if (ImGui::BeginMenu("3D"))
		{
			if (ImGui::MenuItem("test"))
			{
				active = !active;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	

	//Application_Frame();

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void SceneApp::UpdateImGuiIO()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = input_manager_->touch_manager()->is_button_down(0);
	io.MouseDown[1] = input_manager_->touch_manager()->is_button_down(1);

	gef::KeyboardD3D11* keyboard_d3d11 = (gef::KeyboardD3D11*)input_manager_->keyboard();


	io.KeyShift = keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_LSHIFT) || keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_RSHIFT);
	io.KeyCtrl = keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_LCONTROL) || keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_RCONTROL);
	io.KeyAlt = keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_LALT) || keyboard_d3d11->IsKeyDown(gef::Keyboard::KC_RALT);


	for (int key_code_num = 0; key_code_num < gef::Keyboard::NUM_KEY_CODES; ++key_code_num)
	{
		gef::Keyboard::KeyCode key_code = (gef::Keyboard::KeyCode)key_code_num;
		int scan_code = keyboard_d3d11->GetScanCode(key_code);

		int vk_code = MapVirtualKey(scan_code, MAPVK_VSC_TO_VK_EX);

		io.KeysDown[vk_code] = keyboard_d3d11->IsKeyDown(key_code);
		if (keyboard_d3d11->IsKeyPrintable(key_code) && keyboard_d3d11->IsKeyPressed(key_code))
		{
			if (!io.KeyShift && vk_code >= 'A' && vk_code <= 'Z')
				vk_code = 'a' + (vk_code - 'A');
			io.AddInputCharacter(vk_code);
		}
	}
}

void SceneApp::getSpiteFile()
{
	namespace fs = std::experimental::filesystem;
	std::vector<std::string> tempStrings;

	for (auto entry : fs::directory_iterator("Sprite"))
	{
		std::string temp = entry.path().string();
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i] == '\\')
			{
				temp[i] = '/';
			}
		}
		tempStrings.push_back(temp);

	}
		//tempStrings.push_back();
	

	for (int i = 0; i < tempStrings.size(); i++)
	{
		std::vector<std::string> folder;
		for (auto entry : fs::directory_iterator(tempStrings[i]))
		{
			std::string tempString = entry.path().string();
			for (int i = 0; i < tempString.size(); i++)
			{
				if (tempString[i] == '\\')
				{
					tempString[i] = '/';
				}
			}
			folder.push_back(tempString);
			
		}
		lookUp.insert({ gef::GetStringId(tempStrings[i]), tempStrings[i] });
		sprites.insert({ gef::GetStringId(tempStrings[i]), folder });

		//sprite_files.push_back(folder);

	}
		


}
