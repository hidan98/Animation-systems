#include "JSONParser.h"



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

SpriteJSONData* JSONParser::setUpSpriteAnimation(const char* tex, const char* ske)
{
	SpriteJSONData* data = new SpriteJSONData;

	rapidjson::Document tex_Doc;
	tex_Doc = ReadFiles(tex);
	data->texture_atlas = ReadTextureAtlasFromJSON(tex_Doc);

	//free(JSON);

	tex_Doc = ReadFiles(ske);
	data->armiture = static_cast<SpriteArmature*>(ReadArmitureFromJSON(tex_Doc));

	//free(JSON);

	return data;



}

TextureAtlas* JSONParser::ReadTextureAtlasFromJSON(rapidjson::Document& tex_doc)
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

	/*gef::Matrix33 transform;
	transform.SetIdentity();
	transform.SetTranslation(screenPos);

	texture_atlas->baseTransform = scale * transform;*/

	
	const rapidjson::Value& subtexture_array = tex_doc["SubTexture"];
	assert(subtexture_array.IsArray());

	for (int subtex_num = 0; subtex_num < (int)subtexture_array.Size(); subtex_num++)
	{
		SubTexture* subtexture = ReadSubTextureFromJSON(subtexture_array[subtex_num]);
		//subtexture->subtextureTransform = subtexture->subtextureTransform * texture_atlas->baseTransform;
		//may change to store pointers... will nedd to see

		texture_atlas->sub_textures[subtexture->name] = *subtexture;// push_back(*subtexture);
		delete subtexture;
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
	//assert(arm.IsArray());


	for (int subtex_num = 0; subtex_num < (int)arm.Size(); subtex_num++)
	{
		std::string type = arm[subtex_num]["type"].GetString();
		if (type == "Armature")
		{

			/*armiture_struct->type = arm[subtex_num]["type"].GetString();
			armiture_struct->name = arm[subtex_num]["name"].GetString();
			armiture_struct->frameRate = arm[subtex_num]["frameRate"].GetFloat();
			armiture_struct->bones = ReadBoneFromJSON(arm[subtex_num]);
			armiture_struct->skinBone = ReadSkinArmitureFromJSON(arm[subtex_num]);
			armiture_struct->slots = ReadSlotFromJSON(arm[subtex_num]);
			armiture_struct->animations = ReadAnimationDataFromJSON(arm[subtex_num]);*/

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


