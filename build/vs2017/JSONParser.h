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
	static BoneJSONData* setUpBoneAnimation(const char* tex, const char* ske);

private:

	static rapidjson::Document ReadFiles(const char* JSONFileName);
	static SubTexture* ReadSubTextureFromJSON(const rapidjson::Value& subTex);
	static TextureAtlas* ReadTextureAtlasFromJSON(rapidjson::Document& tex_doc);
	static gef::Matrix33 calculateTransform(const SubTexture sub);
	static Armature* ReadArmitureFromJSON(rapidjson::Document& ske_document);

	static Skin* ReadSkinFromJSON(const rapidjson::Value& arm, const char* type);
	static SpriteAnimation* ReadSpriteAnimationFromJSON(const rapidjson::Value& arm, const char* type);
	static std::unordered_map<gef::StringId, Bone*> ReadBoneFromJSON(const rapidjson::Value& arm);
	static Trans ReadTransformFromJSON(const rapidjson::Value& trans);
	static gef::Matrix33 calculateSkinSlotMatrix(Trans transform_);
	static std::unordered_map<gef::StringId, SkinSlot*> ReadSkinArmitureFromJSON(const rapidjson::Value& arm);
	static std::vector<Slot*> ReadSlotFromJSON(const rapidjson::Value& arm);
	static std::vector<BoneAnimation*> ReadAnimationDataFromJSON(const rapidjson::Value& arm);

	
	

};

