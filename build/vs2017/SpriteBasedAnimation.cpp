#include "SpriteBasedAnimation.h"
#include "load_texture.h"


SpriteBasedAnimation::SpriteBasedAnimation() : sprite_texture_(nullptr)
{
}


SpriteBasedAnimation::~SpriteBasedAnimation()
{
}

void SpriteBasedAnimation::init(const std::string tex, const std::string ske, const std::string image, gef::Platform& plat)
{
	//read in the data 
	json_data_ = JSONParser::setUpSpriteAnimation(tex, ske);

	//load in texture
	sprite_texture_ = CreateTextureFromPNG(image.c_str(), plat);

	//create a sprite
	sprite = new gef::Sprite();
	sprite->set_texture(sprite_texture_);

	current_frame = 0;
	time = 0;
}

void SpriteBasedAnimation::update(float dt, gef::Vector2 screenPos)
{
	
	
	if (time > (1.0f / json_data_->armiture->frameRate))
	{
		//if the current frame num is greater than the amount stored
		if (current_frame >= json_data_->armiture->animation->displayFrame.size() - 1)
		{
			//re set to the begining
			current_frame = 0;
			time = 0;
			//re set sprite with texture
			gef::StringId id = json_data_->armiture->skin->display->display_name_id[0];
			spriteSetup(json_data_->texture_atlas->sub_textures[id], screenPos);
		}
		else
		{
			//if we have a next frame
			time = 0;
			current_frame++;
		//get the string id 
			int pos = json_data_->armiture->animation->displayFrame[current_frame];
			gef::StringId id = json_data_->armiture->skin->display->display_name_id[pos];
			//set up sprite 
			spriteSetup(json_data_->texture_atlas->sub_textures[id], screenPos);
			//return sprite_ = spriteSetup(*texture_atlas_, sprite_, screen_pos, name);

		}
	}
	else
	{
		//increase time passed
			time += dt;
		//return sprite_;
	}

	

}

void SpriteBasedAnimation::spriteSetup(SubTexture* sub, gef::Vector2 screenPos)
{	
	//convert from dragonbones draw space to gef
	float posX = screenPos.x + (sub->width * 0.5f - (sub->frameWidth * 0.5f + sub->frameX));
	float posY = screenPos.y + sub->height * 0.5f - (sub->frameHeight * 0.5f + sub->frameY);
	
	//set the position based on calculated above
	sprite->set_position(gef::Vector4(posX, posY, 0.0f));

	//get the right parts of the texture for the sprite
	sprite->set_uv_position(gef::Vector2(sub->x * json_data_->texture_atlas->devideWidth, sub->y * json_data_->texture_atlas->devideHeight));
	sprite->set_uv_width(sub->width * json_data_->texture_atlas->devideWidth);
	sprite->set_uv_height(sub->height * json_data_->texture_atlas->devideHeight);



	//set sprite width and height based on data read in;
	sprite->set_width(sub->width);
	sprite->set_height(sub->height);


}

void SpriteBasedAnimation::cleanUp()
{
	for (auto& it : json_data_->texture_atlas->sub_textures)
	{
		delete it.second;
		it.second = nullptr;
	}

	delete json_data_->texture_atlas;
	json_data_->texture_atlas = nullptr;


	delete json_data_->armiture->animation;
	json_data_->armiture->animation = nullptr;

	delete json_data_->armiture->skin->display;
	json_data_->armiture->skin->display = nullptr;


	delete json_data_->armiture->skin;
	json_data_->armiture->skin = nullptr;

	delete json_data_->armiture;
	json_data_->armiture = nullptr;
	delete json_data_;
	json_data_ = nullptr;

	delete sprite;
	sprite = nullptr;

	delete sprite_texture_;
	sprite_texture_ = nullptr;

}

