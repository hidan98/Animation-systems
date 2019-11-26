#include "SkeletalAnimation2D.h"
#include "JSONParser.h"



SkeletalAnimation2D::SkeletalAnimation2D()
{
}


SkeletalAnimation2D::~SkeletalAnimation2D()                           
{
}

void SkeletalAnimation2D::init(const char* tex, const char* ske, const char* image, gef::Platform& plat)
{

	json_data_ = JSONParser::setUpBoneAnimation(tex, ske);

	updateBoneTransform(json_data_->bone_armiture->bones);
	updateFinalTransform(json_data_->bone_armiture->slot);

}

void SkeletalAnimation2D::updateBoneTransform(std::unordered_map<gef::StringId, Bone*> bones)
{
	for (std::unordered_map<gef::StringId, Bone*>::iterator it=bones.begin(); it != bones.end(); ++it)
	{
		if (it->second->parentStringId != 0)
		{

			it->second->worldBoneMatrix = it->second->local_transform * bones[it->second->parentStringId]->worldBoneMatrix;

		}
		else
			it->second->worldBoneMatrix = it->second->local_transform;
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
		boneTrans = json_data_->bone_armiture->bones[slot[i]->parentStringId]->worldBoneMatrix;

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
