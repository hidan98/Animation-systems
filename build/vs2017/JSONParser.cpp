#include "JSONParser.h"
#include "maths/matrix33.h"
#include "maths/math_utils.h"
#include "system/string_id.h"


JSONParser::JSONParser()
{
}


JSONParser::~JSONParser()
{
}


rapidjson::Document JSONParser::ReadFiles(const char* JSONFileName)
{
	char* JSON = LoadJSON(JSONFileName);
	rapidjson::Document document;

	document.Parse(JSON);
	//assert(document.IsObject());
	free(JSON);

	return document;


}

SpriteJSONData* JSONParser::setUpSpriteAnimation(const std::string tex, const std::string ske)
{
	SpriteJSONData* data = new SpriteJSONData;

	rapidjson::Document tex_Doc;
	tex_Doc = ReadFiles(tex.c_str());
	data->texture_atlas = ReadTextureAtlasFromJSON(tex_Doc, gef::Vector2(0,0));

	//free(JSON);

	tex_Doc = ReadFiles(ske.c_str());
	data->armiture = static_cast<SpriteArmature*>(ReadArmitureFromJSON(tex_Doc));

	//free(JSON);

	return data;



}

BoneJSONData* JSONParser::setUpBoneAnimation(const std::string tex, const std::string ske, gef::Vector2 screenPos)
{
	BoneJSONData* data = new BoneJSONData;
	rapidjson::Document tex_Doc;
	tex_Doc = ReadFiles(tex.c_str());

	data ->texture_atlas = ReadTextureAtlasFromJSON(tex_Doc, screenPos);

	tex_Doc = ReadFiles(ske.c_str());
	data->bone_armiture = static_cast<BoneArmature*>(ReadArmitureFromJSON(tex_Doc));


	return data;

}

TextureAtlas* JSONParser::ReadTextureAtlasFromJSON(rapidjson::Document& tex_doc, gef::Vector2 screenPos)
{

	TextureAtlas* texture_atlas = new TextureAtlas();
	if(tex_doc.HasMember("name"))
		texture_atlas->name = tex_doc["name"].GetString();
	if(tex_doc.HasMember("height"))
		texture_atlas->height = tex_doc["height"].GetFloat();
	if(tex_doc.HasMember("width"))
		texture_atlas->width = tex_doc["width"].GetFloat();
	if(tex_doc.HasMember("imagePath"))
		texture_atlas->imagePath = tex_doc["imagePath"].GetString();

	gef::Matrix33 scale;
	scale.SetIdentity();
	scale.Scale(gef::Vector2(0.50f, 0.50f));

	gef::Matrix33 transform;
	transform.SetIdentity();
	transform.SetTranslation(screenPos);

	texture_atlas->baseTransform = scale * transform;

	
	const rapidjson::Value& subtexture_array = tex_doc["SubTexture"];
	assert(subtexture_array.IsArray());

	for (int subtex_num = 0; subtex_num < (int)subtexture_array.Size(); subtex_num++)
	{
		SubTexture* subtexture = ReadSubTextureFromJSON(subtexture_array[subtex_num]);
		//subtexture->subtextureTransform = subtexture->subtextureTransform * texture_atlas->baseTransform;
		//may change to store pointers... will nedd to see

		texture_atlas->sub_textures[subtexture->nameId] = subtexture;// push_back(*subtexture);
	}
	return texture_atlas;

}

SubTexture* JSONParser::ReadSubTextureFromJSON(const rapidjson::Value& subTex)
{

	SubTexture* subTexture = new SubTexture;


	if (subTex.HasMember("frameX"))
		subTexture->frameX = subTex["frameX"].GetFloat();
	else
		subTexture->frameX = 0;

	if (subTex.HasMember("frameY"))
		subTexture->frameY = subTex["frameY"].GetFloat();
	else
		subTexture->frameY = 0;

	if (subTex.HasMember("width"))
		subTexture->width = subTex["width"].GetFloat();
	else
		subTexture->width = 0;

	if (subTex.HasMember("height"))
		subTexture->height = subTex["height"].GetFloat();
	else
		subTexture->height = 0;


	if (subTex.HasMember("frameWidth"))
		subTexture->frameWidth = subTex["frameWidth"].GetFloat();
	else
		subTexture->frameWidth = subTexture->width;


	if (subTex.HasMember("frameHeight"))
		subTexture->frameHeight = subTex["frameHeight"].GetFloat();
	else
		subTexture->frameHeight = subTexture->height;

	subTexture->name = subTex["name"].GetString();
	subTexture->nameId = gef::GetStringId(subTexture->name);

	subTexture->x = subTex["x"].GetFloat();
	subTexture->y = subTex["y"].GetFloat();

	subTexture->subtextureTransform = calculateTransform(*subTexture);
	return subTexture;
}



gef::Matrix33 JSONParser::calculateTransform(const SubTexture sub)
{
	gef::Matrix33 scale;
	scale.Scale(gef::Vector2(sub.width, sub.height));
	gef::Matrix33 trans;

	float spriteX = sub.width*0.5f - (sub.frameWidth*0.5f + sub.frameX);
	float spriteY = sub.height*0.5f - (sub.frameHeight*0.5f + sub.frameY);
	trans.SetIdentity();
	trans.SetTranslation(gef::Vector2(spriteX, spriteY));

	return scale * trans;
}


Armature* JSONParser::ReadArmitureFromJSON(rapidjson::Document& ske_document)
{	

	const rapidjson::Value& arm = ske_document["armature"].GetArray();


	for (int subtex_num = 0; subtex_num < (int)arm.Size(); subtex_num++)
	{
		std::string type = arm[subtex_num]["type"].GetString();
		if (type == "Armature")
		{
			BoneArmature* armiture_struct = new BoneArmature;
			armiture_struct->type = arm[subtex_num]["type"].GetString();
			armiture_struct->name = arm[subtex_num]["name"].GetString();
			armiture_struct->frameRate = arm[subtex_num]["frameRate"].GetFloat();
			armiture_struct->boneData = ReadBoneFromJSON(arm[subtex_num]);
			armiture_struct->skin = ReadSkinArmitureFromJSON(arm[subtex_num]);
			armiture_struct->slot = ReadSlotFromJSON(arm[subtex_num]);
			armiture_struct->animation = ReadAnimationDataFromJSON(arm[subtex_num]);
			return armiture_struct;
		}
		else if (type == "Sheet")
		{
			SpriteArmature* armiture_struct = new SpriteArmature();

			
			armiture_struct->type = arm[subtex_num]["type"].GetString();
			armiture_struct->name = arm[subtex_num]["name"].GetString();
			armiture_struct->frameRate = arm[subtex_num]["frameRate"].GetFloat();
			armiture_struct->skin = ReadSkinFromJSON(arm[subtex_num], "Sheet");
			armiture_struct->animation = ReadSpriteAnimationFromJSON(arm[subtex_num], type.c_str());
			return armiture_struct;
		}

	}	

}

Skin* JSONParser::ReadSkinFromJSON(const rapidjson::Value& arm, const char* type)
{
	const rapidjson::Value& skin_stuff = arm["skin"];
	Skin* skin_struct = new Skin();
	

	for (int subtex_num = 0; subtex_num < (int)skin_stuff.Size(); subtex_num++)
	{
		const rapidjson::Value& slot = skin_stuff[subtex_num]["slot"];
		for (int moreLoop = 0; moreLoop < (int)slot.Size(); moreLoop++)
		{
			const rapidjson::Value& display_JSON = slot[moreLoop]["display"];
			Display* display = new Display();
			skin_struct->display = display;


			for (int i = 0; i < (int)display_JSON.Size(); i++)
			{
				std::string temp = display_JSON[i]["name"].GetString();
				skin_struct->display->display_name.push_back(temp);
				skin_struct->display->display_name_id.push_back(gef::GetStringId(temp));
			}
		}
	}
	return skin_struct;
}

SpriteAnimation* JSONParser::ReadSpriteAnimationFromJSON(const rapidjson::Value& arm, const char* type)
{
	SpriteAnimation* animiation_struct = new SpriteAnimation;
	const rapidjson::Value& animation = arm["animation"];


	for (int subtex_num = 0; subtex_num < (int)animation.Size(); subtex_num++)
	{

		if (animation[subtex_num].HasMember("duration"))
			animiation_struct->duration = animation[subtex_num]["duration"].GetFloat();
		if (animation[subtex_num].HasMember("name"))
			animiation_struct->name = animation[subtex_num]["name"].GetString();

		const rapidjson::Value& slot = animation[subtex_num]["slot"];

		for (int i = 0; i < (int)slot.Size(); i++)
		{

			if (std::string(type) == "Sheet")
			{				
				const rapidjson::Value& displayFrame = slot[i]["displayFrame"];
				
				for (int j = 0; j < (int)displayFrame.Size(); j++)
				{
					if (j == 0)
					{
						animiation_struct->displayFrame.push_back(0);
					}
					else
					{
						float temp = displayFrame[j]["value"].GetFloat();
						animiation_struct->displayFrame.push_back(temp);
					}
				}
			}	

		}

	}
	return animiation_struct;
}

BoneDataStruct JSONParser::ReadBoneFromJSON(const rapidjson::Value& val)
{
	std::unordered_map<gef::StringId, Bone*> bone_map;
	std::vector<Bone*> bone_vec;

	const rapidjson::Value& bones = val["bone"];

	for (int i = 0; i < (int)bones.Size(); i++)
	{
		Bone* tempBone = new Bone;

		tempBone->name = bones[i]["name"].GetString();
		tempBone->nameStringId = gef::GetStringId(tempBone->name);


		if (bones[i].HasMember("parent"))
		{
			tempBone->parent = bones[i]["parent"].GetString();
			tempBone->parentStringId = gef::GetStringId(tempBone->parent);	
		}
			


		if (bones[i].HasMember("transform"))
			tempBone->transform = ReadTransformFromJSON(bones[i]["transform"]);
		else
			tempBone->transform = Trans();

		tempBone->local_transform = calculateSkinSlotMatrix(tempBone->transform);

		bone_map[tempBone->nameStringId] = tempBone;
		bone_vec.push_back(tempBone);
	}

	BoneDataStruct temp;
	temp.bonesMap = bone_map;
	temp.bonesVec = bone_vec;
	return temp;

}


Trans JSONParser::ReadTransformFromJSON(const rapidjson::Value& trans)
{
	Trans temp_trans;
	temp_trans.x = trans["x"].GetFloat();
	temp_trans.y = trans["y"].GetFloat();
	if (trans.HasMember("skX"))
		temp_trans.sk = trans["skX"].GetFloat();

	return temp_trans;
}


gef::Matrix33 JSONParser::calculateSkinSlotMatrix(Trans transform_)
{
	gef::Matrix33 pos;
	pos.SetIdentity();
	pos.SetTranslation(gef::Vector2(transform_.x, transform_.y));

	gef::Matrix33 rotate;
	rotate.SetIdentity();
	float rad = gef::DegToRad(transform_.sk);
	if (rad != 0)
		rotate.Rotate(rad);
	return rotate * pos;

}
std::unordered_map<gef::StringId, SkinSlot*> JSONParser::ReadSkinArmitureFromJSON(const rapidjson::Value& arm)
{
	std::unordered_map<gef::StringId, SkinSlot*>  skinSlotMap;

	const rapidjson::Value& skin_data = arm["skin"];

	for (int i = 0; i < (int)skin_data.Size(); i++)
	{

		const rapidjson::Value& slot_data = skin_data[i]["slot"];
		for (int j = 0; j < (int)slot_data.Size(); j++)
		{
			SkinSlot* skin_slot_struct = new SkinSlot;
			skin_slot_struct->name = slot_data[j]["name"].GetString();
			skin_slot_struct->nameId = gef::GetStringId(skin_slot_struct->name);

			const rapidjson::Value& display_data = slot_data[j]["display"];

			for (int k = 0; k < (int)display_data.Size(); k++)
			{
				skin_slot_struct->display_name = display_data[k]["name"].GetString();
				skin_slot_struct->displayId = gef::GetStringId(skin_slot_struct->display_name);
				skin_slot_struct->transform = ReadTransformFromJSON(display_data[k]["transform"]);
				skin_slot_struct->transformMatrix = calculateSkinSlotMatrix(skin_slot_struct->transform);
				skinSlotMap[skin_slot_struct->nameId] = skin_slot_struct;//				skinSlotMap.push_back(skin_slot_struct);
			}
		}
	}
	return skinSlotMap;
}

std::vector<Slot*> JSONParser::ReadSlotFromJSON(const rapidjson::Value& arm)
{
	std::vector<Slot*> slot_vec;
	const rapidjson::Value& slot = arm["slot"];
	//slot_vec->resize((int)slot.Size());

	for (int i = 0; i < (int)slot.Size(); i++)
	{
		Slot* temp_slot = new Slot;
		if (slot[i].HasMember("name"))
		{
			temp_slot->name = slot[i]["name"].GetString();
			temp_slot->nameStringId = gef::GetStringId(temp_slot->name);
		}
			

		if (slot[i].HasMember("parent"))
		{
			temp_slot->parent = slot[i]["parent"].GetString();
			temp_slot->parentStringId = gef::GetStringId(temp_slot->parent);

		}
			

		slot_vec.push_back(temp_slot);
	}
	return slot_vec;

}


std::vector<BoneAnimation*> JSONParser::ReadAnimationDataFromJSON(const rapidjson::Value& arm)
{
	std::vector<BoneAnimation*> animation_bone_data_vec;

	const rapidjson::Value& animation = arm["animation"];


	for (int i = 0; i < (int)animation.Size(); i++)
	{
		BoneAnimation* tempAnimationData = new BoneAnimation;
		tempAnimationData->name = animation[i]["name"].GetString();
		tempAnimationData->duration = animation[i]["duration"].GetFloat();

		const rapidjson::Value& animationBone = animation[i]["bone"];

		for (int j = 0; j < (int)animationBone.Size(); j++)
		{
			AnimationBoneData* temp_animation_bone_data = new AnimationBoneData;
			temp_animation_bone_data->name = animationBone[j]["name"].GetString();
			temp_animation_bone_data->nameId = gef::GetStringId(temp_animation_bone_data->name);
			//temp_animation_bone_data->duration = animationBone[j]["duration"].GetFloat();
			float start_time = 0;
			if (animationBone[j].HasMember("translateFrame"))
			{
				const rapidjson::Value& translateFrame = animationBone[j]["translateFrame"];

				for (int k = 0; k < (int)translateFrame.Size(); k++)
				{
					translationFrameData tempFrameData ;
					if (translateFrame[k].HasMember("duration"))
					{
						tempFrameData.duration = translateFrame[k]["duration"].GetFloat();
						tempFrameData.startTime = start_time;
						start_time += tempFrameData.duration;
					}
						

					if (translateFrame[k].HasMember("x"))
						tempFrameData.XY.x = translateFrame[k]["x"].GetFloat();

					if (translateFrame[k].HasMember("y"))
						tempFrameData.XY.y = translateFrame[k]["y"].GetFloat();

					temp_animation_bone_data->translations.push_back(tempFrameData);
				}
			}
			else
				temp_animation_bone_data->translations.push_back(translationFrameData());


			if (animationBone[j].HasMember("rotateFrame"))
			{
				const rapidjson::Value& rotateFrame = animationBone[j]["rotateFrame"];

				start_time = 0;
				for (int k = 0; k < (int)rotateFrame.Size(); k++)
				{
					rotateFrameData tempRotateData;

					if (rotateFrame[k].HasMember("duration"))
					{
						tempRotateData.duration = rotateFrame[k]["duration"].GetFloat();
						tempRotateData.startTime = start_time;
						start_time += tempRotateData.duration;
					}
						

					if (rotateFrame[k].HasMember("rotate"))
						tempRotateData.rotate = rotateFrame[k]["rotate"].GetFloat();


					temp_animation_bone_data->rotations.push_back(tempRotateData);
				}
			}
			else
				temp_animation_bone_data->rotations.push_back(rotateFrameData());

			tempAnimationData->animation_bone_data.push_back(temp_animation_bone_data);
		}

		animation_bone_data_vec.push_back(tempAnimationData);
	}


	return animation_bone_data_vec;
}