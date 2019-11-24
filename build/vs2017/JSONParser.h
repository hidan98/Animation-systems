#pragma once
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "load_texture.h"
#include "load_json.h"
#include "AnimationData.h"

class JSONParser
{
public:
	JSONParser();
	~JSONParser();
	static SpriteJSONData* setUpSpriteAnimation(const char* tex, const char* ske);


private:

	static rapidjson::Document ReadFiles(const char* JSONFileName);
	static SubTexture* ReadSubTextureFromJSON(const rapidjson::Value& subTex);
	static TextureAtlas* ReadTextureAtlasFromJSON(rapidjson::Document& tex_doc);
	static gef::Matrix33 calculateTransform(const SubTexture sub);
	static Armature* ReadArmitureFromJSON(rapidjson::Document& ske_document);

	static Skin* ReadSkinFromJSON(const rapidjson::Value& arm, const char* type);
	static SpriteAnimation* ReadSpriteAnimationFromJSON(const rapidjson::Value& arm, const char* type);


	

};

