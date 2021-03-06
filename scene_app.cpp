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

#include <platform/d3d11/system/platform_d3d11.h>
#include <platform\d3d11\input\keyboard_d3d11.h>
#include "imgui_user.h"

#include <experimental\filesystem>
#include <vector>
#include <string>
#include <graphics/scene.h>
#include "Animation_Utils.h"

#include "picking.h"
#include "ccd.h"




SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	input_manager_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	worldPhysics(nullptr),
	anim(nullptr),
	bone_(nullptr),
	graph(nullptr)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	renderer_3d_ = gef::Renderer3D::Create(platform_);
	InitFont();
	SetupCamera();
	SetupLights();
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	InitFont();

	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	active = false;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	gef::PlatformD3D11& platform_d3d = static_cast<gef::PlatformD3D11&>(platform_);

	ImGui_ImplWin32_Init(platform_d3d.hwnd());
	ImGui_ImplDX11_Init(platform_d3d.device(), platform_d3d.device_context());

	//Application_Initialize();
	getSpiteFile();

	
	primitive_renderer_ = new PrimitiveRenderer(platform_);
	

	active_graph = false;
	done = false;


	modelSelected = false;
	effector_position_ = new gef::Vector4(0, 0, 0);

	sprites = Animation_Utils::ReadSprites();

	Active3D = false;
}


void SceneApp::CleanUp()
{
	//Application_Finalize();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();



	delete input_manager_;
	input_manager_ = NULL;

	CleanUpFont();

	delete sprite_renderer_;
	sprite_renderer_ = NULL;


	delete renderer_3d_;
	renderer_3d_ = NULL;
	
	if (worldPhysics)
	{
		worldPhysics->cleanUp();
		delete worldPhysics;
		worldPhysics = nullptr;
	}
	
	if (anim)
	{
		anim->cleanUp();
		delete anim;
		anim = nullptr;
	}

	if (bone_)
	{
		bone_->cleanUp();
		delete bone_;
		bone_ = nullptr;
	}

	if (graph)
	{
		graph->cleanup();
		delete graph;
		graph = nullptr;
	}

	if (effector_position_)
	{
		delete effector_position_;
		effector_position_ = nullptr;
	}

	if (primitive_renderer_)
	{
		delete primitive_renderer_;
		primitive_renderer_ = nullptr;
	}
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;
	time = frame_time;

	input_manager_->Update();
	UpdateImGuiIO();


	if (Active3D)
	{
		if (graph->output)
		{
			graph->updateOut(frame_time);
			
			
			graph->update(time);
			

		
			graph->getCurrent()->skinnedMesh->UpdateBoneMatrices(graph->output->getOutput());		

		
			

		}
	}


	gef::Vector2 mouse_pos(gef::Vector2::kZero);
	bool mb_down = false;

	if (input_manager_)
	{

		mouse_pos = input_manager_->touch_manager()->mouse_position();
		mb_down = input_manager_->touch_manager()->is_button_down(1);
	}

	if (mb_down)
	{
		gef::Vector4 mouse_ray_start_point, mouse_ray_direction;
		//mouse_position = gef::Vector2(600, 300);
		GetScreenPosRay(mouse_pos, renderer_3d_->projection_matrix(), renderer_3d_->view_matrix(), mouse_ray_start_point, mouse_ray_direction, (float)platform_.width(), (float)platform_.height(), ndc_zmin_);

		RayPlaneIntersect(mouse_ray_start_point, mouse_ray_direction, gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 1.0f), *effector_position_);
	}

	if(anim)
		anim->update(frame_time, gef::Vector2(platform_.width()*0.5f, platform_.height()*0.5f));

	if(bone_)
		bone_->update(frame_time, gef::Vector2(platform_.width()*0.5f, platform_.height()*0.5f));

	if(worldPhysics)
		worldPhysics->update(frame_time);

	return true;
}




void SceneApp::Render()
{


	SetCameraMatrices();
	// draw meshes here
	renderer_3d_->Begin();


	if (worldPhysics)
		worldPhysics->render(renderer_3d_);

	if (Active3D)
	{
		if (graph->getCurrent()->skinnedMesh)
		{
			gef::Matrix44 player_trans;
			player_trans.SetIdentity();
			gef::Matrix44 scale;
			scale.Scale(gef::Vector4(0.01f, 0.01f, 0.01f));
			graph->getCurrent()->skinnedMesh->set_transform(scale * player_trans);
			renderer_3d_->DrawSkinnedMesh(*graph->getCurrent()->skinnedMesh, graph->getCurrent()->skinnedMesh->bone_matrices());
		}
	}
	

	//primitive_renderer_->Reset();

	//RenderEndEffector();

	//primitive_renderer_->Render(*renderer_3d_);

	renderer_3d_->End();
	sprite_renderer_->Begin(false);


	if(anim)
		sprite_renderer_->DrawSprite(*anim->getSprite());

	if(bone_)
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
void SceneApp::SetCameraMatrices()
{
	// setup view and projection matrices
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_, camera_lookat_, camera_up_);

	if (renderer_3d_)
	{
		renderer_3d_->set_projection_matrix(projection_matrix);
		renderer_3d_->set_view_matrix(view_matrix);
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
				for (int i =0; i < sprites.size(); i++)
				{
					if (ImGui::MenuItem(sprites[i].name.c_str()))
					{
						
						if (anim)
							anim->cleanUp();
						else
							anim = new SpriteBasedAnimation();

						anim->init(sprites[i].tex.c_str(), sprites[i].ske.c_str(), sprites[i].png.c_str(), platform());
					}
				}

				if (ImGui::MenuItem("Stop Sprits"))
				{
					anim->cleanUp();
					delete anim;
					anim = nullptr;
				}
					

				ImGui::EndMenu();
			}


			if (bone_)
			{
				if (ImGui::BeginMenu("Bone Animation"))
				{
					for (int i = 0; i < bone_->getData()->bone_armiture->animation.size(); i++)
					{
						if (ImGui::MenuItem(bone_->getData()->bone_armiture->animation[i]->name.c_str()))
						{
							if (!bone_)
							{
								bone_ = new SkeletalAnimation2D();
								bone_->init("Dragon_tex.json", "Dragon_ske.json", "Dragon_tex.png", platform_);
							}
							bone_->setAnimationNum(i);
						}
					}

					if (bone_)
					{
						if (ImGui::MenuItem("stop Bone Animation"))
						{
							bone_->cleanUp();
							delete bone_;
							bone_ = nullptr;
						}
					}
					ImGui::EndMenu();
				}
			}
			else
			{
				if (ImGui::MenuItem("Begin bone 2D"))
				{
					bone_ = new SkeletalAnimation2D();
					bone_->init("Dragon_tex.json", "Dragon_ske.json", "Dragon_tex.png", platform_);
				}
			}
			
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("3D"))
		{
			
			if (!Active3D)
			{
				if (ImGui::MenuItem("Start 3D"))
				{
					worldPhysics = new Physics();
					worldPhysics->init(&platform_, renderer_3d_);
					graph = new nodeGraph(&platform_, worldPhysics->getWorld());
					graph->init(effector_position_, &variable_table);


					Active3D = true;
				}
				
			}

			if (Active3D)
			{
				if (modelSelected)
				{
					if (ImGui::MenuItem("Graph"))
					{
						active_graph = !active_graph;
					}
				}
				

				if (ImGui::BeginMenu("select model"))
				{
					for (int i = 0; i < graph->getModels().size(); i++)
					{
						if (ImGui::MenuItem(graph->getModels()[i].c_str()))
						{
							graph->setUpModel(i);
							modelSelected = true;
						}
					}

					ImGui::EndMenu();
				}

				if (modelSelected)
				{
					if (ImGui::MenuItem("Stop 3D"))
					{
						cleanUp3d();
						Active3D = false;
						modelSelected = false;
					}
				}
							
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	if (graph)
	{
		if (active_graph)
		{
			ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowBgAlpha(0.95f);
			if (ImGui::Begin("Example: Custom Node Graph", &active_graph, ImGuiWindowFlags_NoScrollbar))
			{

				graph->render();

			

			}
			ImGui::End();
		}
	}


	if (Active3D)
	{
		ImGui::Begin("variable table");


		for (auto& it : variable_table)
		{
			if (it.second.type == dataType::Real)
				ImGui::SliderFloat(it.second.name.c_str(), &it.second.floatData, it.second.min, it.second.max);
			else if (it.second.type == dataType::boolean)
				ImGui::Checkbox(it.second.name.c_str(), &it.second.toggle);

		}

		ImGui::End();
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
	

}

void SceneApp::SetupCamera()
{
	// initialise the camera settings
	camera_eye_ = gef::Vector4(-1.0f, 1.0f, 4.0f);
	camera_lookat_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 100.f;
	SetCameraMatrices();

}

void SceneApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void SceneApp::cleanUp3d()
{
	if (graph)
	{
		graph->cleanup();
		delete graph;
		graph = nullptr;
	}

	if (worldPhysics)
	{
		worldPhysics->cleanUp();
		delete worldPhysics;
		worldPhysics = nullptr;
	}
		
	



}