#include "SpriteBasedAnimation.h"
#include "load_texture.h"


SpriteBasedAnimation::SpriteBasedAnimation() : sprite_texture_(nullptr)
{
}


SpriteBasedAnimation::~SpriteBasedAnimation()
{
}

void SpriteBasedAnimation::init(const char* tex, const char* ske, const char* image, gef::Platform& plat)
{

	json_data_ = JSONParser::setUpSpriteAnimation(tex, ske);

	sprite_texture_ = CreateTextureFromPNG(image, plat);

	sprite = new gef::Sprite();
	sprite->set_texture(sprite_texture_);

	current_frame = 0;
	time = 0;
}

void SpriteBasedAnimation::update(float dt, gef::Vector2 screenPos)
{

	if (json_data_->armiture->type == "Sheet")
	{
		if (time > (1.0f / json_data_->armiture->frameRate))
		{
			if (current_frame >= json_data_->armiture->animation->displayFrame.size() - 1)
			{
				current_frame = 0;
				time = 0;
				//std::string name = armature_->skin->display_name[0];
				gef::StringId id = json_data_->armiture->skin->display->display_name_id[0];
				spriteSetup(json_data_->texture_atlas->sub_textures[id], screenPos);
			}
			else
			{
				time = 0;
				current_frame++;

				int pos = json_data_->armiture->animation->displayFrame[current_frame];
				gef::StringId id = json_data_->armiture->skin->display->display_name_id[pos];
				//std::string name = json_data_->armiture->skin->display->display_name[pos];// armature_->skin->display_name[pos];
				spriteSetup(json_data_->texture_atlas->sub_textures[id], screenPos);
				//return sprite_ = spriteSetup(*texture_atlas_, sprite_, screen_pos, name);

			}
		}
		else
		{
			time += dt;
			//return sprite_;
		}

	}

}

void SpriteBasedAnimation::spriteSetup(SubTexture* sub, gef::Vector2 screenPos)
{
	

	float posX = screenPos.x + (sub->width * 0.5f - (sub->frameWidth * 0.5f + sub->frameX));
	float posY = screenPos.y + sub->height * 0.5f - (sub->frameHeight * 0.5f + sub->frameY);



	sprite->set_position(gef::Vector4(posX, posY, 0.0f));


	sprite->set_uv_position(gef::Vector2(sub->x / json_data_->texture_atlas->width, sub->y / json_data_->texture_atlas->height));
	sprite->set_uv_width(sub->width / json_data_->texture_atlas->width);
	sprite->set_uv_height(sub->height / json_data_->texture_atlas->height);




	sprite->set_width(sub->width);
	sprite->set_height(sub->height);


}

