#include "SkeletalAnimation2D.h"
#include "JSONParser.h"
#include "maths\math_utils.h"
#include "graphics\sprite_renderer.h"

SkeletalAnimation2D::SkeletalAnimation2D() : sprite_texture(nullptr)
{
}


SkeletalAnimation2D::~SkeletalAnimation2D()                           
{
}

void SkeletalAnimation2D::init(const char* tex, const char* ske, const char* image, gef::Platform& plat)
{

	json_data_ = JSONParser::setUpBoneAnimation(tex, ske, gef::Vector2(plat.width()/2, plat.height()/2));

	updateBoneTransform(json_data_->bone_armiture->boneData);
	updateFinalTransform(json_data_->bone_armiture->slot);

	sprite_texture = CreateTextureFromPNG(image, plat);

	for (unsigned i = 0; i < json_data_->texture_atlas->sub_textures.size(); i++)
	{
		gef::Sprite* temp = new gef::Sprite();
		temp->set_texture(sprite_texture);
		sprites.push_back(temp);

	}

}

void SkeletalAnimation2D::updateBoneTransform(BoneDataStruct bones)
{
	for (auto it : bones.bonesVec)
	{
		if (it->parentStringId != 0)
		{
			gef::Matrix33 temp = bones.bonesMap[it->parentStringId]->worldBoneMatrix;
			it->worldBoneMatrix = it->local_transform * temp;

		}
		else
			it->worldBoneMatrix = it->local_transform;
	}
}


void SkeletalAnimation2D::updateFinalTransform(std::vector<Slot*> slot)
{

	for (unsigned i = 0; i < slot.size(); i++)
	{
		//gef::Matrix33 
		gef::Matrix33 subTexTrans;
		gef::Matrix33 sprite_offset;
		gef::Matrix33 boneTrans;


		gef::StringId temp = json_data_->bone_armiture->skin[slot[i]->parentStringId]->displayId;
		subTexTrans = json_data_->texture_atlas->sub_textures[temp]->subtextureTransform;



		sprite_offset = json_data_->bone_armiture->skin[slot[i]->parentStringId]->transformMatrix;
		

			
		/*for (unsigned j = 0; j < armature_->skinBone->size(); j++)
		{

			if (slot[i].parent == armature_->skinBone->at(j).name)
			{
				sprite_offset = armature_->skinBone->at(j).transformMatrix;
				break;
			}
		}*/
		boneTrans = json_data_->bone_armiture->boneData.bonesMap[slot[i]->parentStringId]->worldBoneMatrix;

		/*for (unsigned j = 0; j < armature_->bones->size(); j++)
		{

			if (slot[i].parent == armature_->bones->at(j).name)
			{
				boneTrans = armature_->bones->at(j).worldBoneMatrix;
				break;
			}
		}*/

		slot[i]->finalTransform = subTexTrans * sprite_offset * boneTrans * json_data_->texture_atlas->baseTransform;
	}

}

void SkeletalAnimation2D::update(float dt, gef::Vector2 screenPos)
{
	updateBones(json_data_->bone_armiture->animation, json_data_->bone_armiture->boneData.bonesMap, dt);
	updateFinalTransform(json_data_->bone_armiture->slot);

	for (unsigned i = 0; i < json_data_->bone_armiture->slot.size(); i++)
	{
		gef::StringId nameId = json_data_->bone_armiture->skin[json_data_->bone_armiture->slot[i]->parentStringId]->displayId;// armature_->slots->at(i).parent;
		

		for (int j = 0; j < json_data_->bone_armiture->skin.size(); j++)
		{
			
			spriteSetup(json_data_->texture_atlas->sub_textures[nameId], screenPos, sprites[i]);
		}

	}



}

void SkeletalAnimation2D::spriteSetup(SubTexture* sub, gef::Vector2 screenPos, gef::Sprite* sprite)
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


void SkeletalAnimation2D::updateBones(std::vector<BoneAnimation*> animation, std::unordered_map<gef::StringId, Bone*> bones, float dt)
{
	int val = 2;
	for (unsigned i = 0; i < animation[val]->animation_bone_data.size(); i++)
	{
		gef::StringId id = animation[val]->animation_bone_data[i]->nameId;


		Bone* bone = bones[id];

		float rotate = getRotLerp(animation[val]->animation_bone_data[i], dt);

		float newAngle = bone->transform.sk + rotate;

		bone->local_transform.Rotate(gef::DegToRad(newAngle));


		gef::Vector2 lerpPos = getTransLerp(animation[val]->animation_bone_data[i], dt);


		gef::Vector2 tempVec2 = gef::Vector2(bone->transform.x, bone->transform.y);


		gef::Vector2 temp3 = gef::Vector2(tempVec2 + lerpPos);
		bone->local_transform.SetTranslation(temp3);
	}

	updateBoneTransform(json_data_->bone_armiture->boneData);
}


gef::Vector2 SkeletalAnimation2D::getTransLerp(AnimationBoneData* data, float dt)
{
	int transPos = data->translationFramePos;

	if (data->translations.size() > 1)
	{
		if (data->translations[transPos].duration > 0)
			data->translationFrameTime = data->translationFrameTime + 1 % data->translations[transPos].duration;
		else
			data->translationFrameTime = 0;

		if (data->translationFrameTime >= (data->translations[transPos].duration))
		{
			//animation_data.translationFrameTime = 0;

			if (data->translationFramePos + 1 > data->translations.size() - 1)
			{
				data->translationFramePos = 0;
			}

			else
			{
				data->translationFramePos++;				
			}
			data->translationFrameTime = 0;
		}


		transPos = data->translationFramePos;

		gef::Vector2 temp = data->translations[transPos].XY;

		gef::Vector2 temp2;
		if (transPos + 1 > data->translations.size() - 1)
			temp2 = data->translations[0].XY;
		else
			temp2 = data->translations[transPos + 1].XY;

		float denominator = (float)data->translations[transPos].duration;
		float time = 0;
		if (denominator > 0.0f)
			time = (float)data->translationFrameTime / denominator;

		float X = gef::Lerp(temp.x, temp2.x, time);
		float Y = gef::Lerp(temp.y, temp2.y, time);
		return gef::Vector2(X, Y);
	}

	return gef::Vector2(0, 0);


}





float SkeletalAnimation2D::getRotLerp(AnimationBoneData* data, float dt)
{
	int rotatePos = data->rotationFramePos;


	//check to see if there are more than one animation key frames
	if (data->rotations.size() > 1)
	{
		//check to see if the current animation duration is grater than 0, i its 0 its the end of the animation and reset time
		if (data->rotations[rotatePos].duration > 0)
		{
			data->rotationFrameTime = data->rotationFrameTime + 1 % data->rotations[rotatePos].duration;
		}			
		else
			data->rotationFrameTime = 0;


		//if the current animation time is greate than the duration we change the animation frame
		if (data->rotationFrameTime >= data->rotations[rotatePos].duration)
		{
			if (data->rotationFramePos + 1 > data->rotations.size() - 1)
				data->rotationFramePos = 0;
			else
				data->rotationFramePos++;

			data->rotationFrameTime = 0;
		}

		//get the animation key we are working with, may have changed due to time
		rotatePos = data->rotationFramePos;

		//get the current rotation key frame
		float rotateTemp = data->rotations[rotatePos].rotate;

		//get the next key frame rotation
		float rotateNext;
		if (rotatePos + 1 > data->rotations.size() - 1)
			rotateNext = data->rotations[0].rotate;
		else
			rotateNext = data->rotations[rotatePos + 1].rotate;

		//calculate thr diffrence in rotation
		float angleDiff = rotateNext - rotateTemp;

		//get angle between -180 and 180 to get the shortest distance for rotation
		if (angleDiff > 180)
			angleDiff -= 360;
		else if (angleDiff < -180)
			angleDiff += 360.0f;


		float denominator = (float)data->rotations[rotatePos].duration;
		float time = 0;

		//the duration might be 0 if its the last frame if this is the case the animation time should be set to 0 as its the begining again
		if (denominator > 0.0f)
			time = (float)data->rotationFrameTime / denominator;

		return lerpRot(rotateTemp, angleDiff, time);
	}

	return data->rotations[rotatePos].rotate;

}

float SkeletalAnimation2D::lerpRot(float start, float diff, float t)
{
	return start + (t*diff);
}


void SkeletalAnimation2D::render(gef::SpriteRenderer* renderer)
{

	for (int i =0; i < sprites.size(); i++)
	{
		if (sprites[i])
			renderer->DrawSprite(*sprites[i], json_data_->bone_armiture->slot[i]->finalTransform);
	}

}
