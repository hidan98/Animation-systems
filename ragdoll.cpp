#include "ragdoll.h"

#include <btBulletWorldImporter.h>
#include <system/debug_log.h>

std::string model_name;

Ragdoll::Ragdoll() :
	scale_factor_(1.0f)
{

}

void Ragdoll::Init(const gef::SkeletonPose & bind_pose, btDiscreteDynamicsWorld* dynamics_world, const char * physics_filename)
{
	bind_pose_ = bind_pose;
	pose_ = bind_pose;

	gef::Matrix44 identity;
	identity.SetIdentity();

	bone_rb_offset_matrices_.resize(bind_pose_.skeleton()->joint_count(), identity);
	bone_rbs_.resize(bind_pose_.skeleton()->joint_count(), NULL);
	bone_world_matrices_.resize(bind_pose_.skeleton()->joint_count());


	btBulletWorldImporter* fileLoader = new btBulletWorldImporter(dynamics_world);

	//std::string ragdoll_filename = model_name + "/ragdoll.bullet";
	fileLoader->loadFile(physics_filename);

	int numRigidBodies = fileLoader->getNumRigidBodies();
	for (int i = 0; i < fileLoader->getNumRigidBodies(); i++)
	{
		btCollisionObject* obj = fileLoader->getRigidBodyByIndex(i);
		btRigidBody* body = btRigidBody::upcast(obj);

		// properties

		// The Blender object name
		std::string rb_name(fileLoader->getNameForPointer(body));

		// trim the blender rigid body name to match bone names
		rb_name = std::string(&rb_name.c_str()[sizeof("OBArmature_") - 1]);
		char* new_name = (char*)rb_name.c_str();
		new_name[rb_name.length() - sizeof("_hitbox") + 1] = 0;
		rb_name = std::string(new_name);

		gef::DebugOut("  object name = %s\n", rb_name.c_str());	
		gef::DebugOut("  get position = (%4.3f,%4.3f,%4.3f)\n",
			body->getCenterOfMassPosition().getX(),
			body->getCenterOfMassPosition().getY(),
			body->getCenterOfMassPosition().getZ());			// Blender CoM
		if (body->getInvMass() == 0)
			gef::DebugOut("  static object\n");
		else
		{
			gef::DebugOut("  mass = %4.3f\n", 1 / body->getInvMass());		// Blender Mass
		}
		gef::DebugOut("\n");

		if (bind_pose_.skeleton())
		{
			gef::StringId joint_name_id = gef::GetStringId(rb_name);
			if (joint_name_id != 0)
			{
				// find bone in skeleton that matches the name of the rigid body
				int joint_num = bind_pose_.skeleton()->FindJointIndex(joint_name_id);
				if (joint_num != -1)
				{
					bone_rbs_[joint_num] = body;

					// CALCULATE THE BONE TO RIGID BODY OFFSET MATRIX HERE
					gef::Matrix44 boneWorld_inverse = bind_pose_.global_pose()[joint_num];
					boneWorld_inverse.Inverse(boneWorld_inverse);
					
					boneWorld_inverse.SetTranslation(boneWorld_inverse.GetTranslation() * 0.01);
					gef::Matrix44 rbw_trans = btTransform2Matrix(bone_rbs_[joint_num]->getCenterOfMassTransform());

					bone_rb_offset_matrices_[joint_num] = rbw_trans * boneWorld_inverse;

				}
			}
		}
	}

	delete fileLoader;
	fileLoader = NULL;
}

Ragdoll::~Ragdoll()
{
}

void Ragdoll::UpdatePoseFromRagdoll()
{
	for (int bone_num = 0; bone_num < bind_pose_.skeleton()->joint_count(); ++bone_num)
	{
		const gef::Joint& joint = bind_pose_.skeleton()->joint(bone_num);
		gef::Matrix44 anim_bone_local_transform;


		btRigidBody* bone_rb = bone_rbs_[bone_num];
		if (bone_rb)
		{
			// REPLACE THIS LINE BELOW TO CALCULATE THE BONE LOCAL TRANSFORM
			// BASED ON THE RIGID BODY WORLD TRANSFORM
			gef::Matrix44 rb_offInverse = bone_rb_offset_matrices_[bone_num];
			rb_offInverse.Inverse(rb_offInverse);
			gef::Matrix44 pw_inverse;
			if (joint.parent != -1)
				pw_inverse = bone_world_matrices_[joint.parent];
			else
				pw_inverse.SetIdentity();
			pw_inverse.Inverse(pw_inverse);
			pw_inverse.SetTranslation(pw_inverse.GetTranslation() * 0.01);

			anim_bone_local_transform = rb_offInverse * btTransform2Matrix(bone_rbs_[bone_num]->getCenterOfMassTransform()) * pw_inverse;
			anim_bone_local_transform.SetTranslation(anim_bone_local_transform.GetTranslation() * 100.0f);
		}
		else
		{
			anim_bone_local_transform = bind_pose_.local_pose()[bone_num].GetMatrix();
		}

		// calculate bone world transforms for anim skeleton
		if (joint.parent == -1)
		{
			bone_world_matrices_[bone_num] = anim_bone_local_transform;
		}
		else
		{
			bone_world_matrices_[bone_num] = anim_bone_local_transform * bone_world_matrices_[joint.parent];
		}
	}

	pose_.CalculateLocalPose(bone_world_matrices_);
	pose_.CalculateGlobalPose();
}

void Ragdoll::UpdateRagdollFromPose()
{
	for (int bone_num = 0; bone_num < bind_pose_.skeleton()->joint_count(); ++bone_num)
	{
		const gef::Joint& joint = bind_pose_.skeleton()->joint(bone_num);

		btRigidBody* bone_rb = bone_rbs_[bone_num];
		if (bone_rb)
		{
			// CALCULATE THE RIGID BODY WORLD TRANSFORM BASED ON THE CURRENT SKELETON POSE

			gef::Matrix44 B_WT = pose_.global_pose()[bone_num];
			B_WT.SetTranslation(B_WT.GetTranslation() * 0.01);

			gef::Matrix44 RB_off = bone_rb_offset_matrices_[bone_num];

			gef::Matrix44 rb_world = B_WT * RB_off;
			bone_rb->setCenterOfMassTransform(Matrix2btTransform(rb_world));
			bone_rb->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
			bone_rb->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		}

	}

}

gef::Matrix44 btTransform2Matrix(const btTransform& transform)
{
	gef::Matrix44 result;

	btQuaternion rotation = transform.getRotation();
	btVector3 position = transform.getOrigin();

	result.Rotation(gef::Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w()));
	result.SetTranslation(gef::Vector4(position.x(), position.y(), position.z()));

	return result;
}

btTransform Matrix2btTransform(const gef::Matrix44& mtx)
{
	gef::Vector4 mtx_position = mtx.GetTranslation();

	gef::Quaternion mtx_rot;
	mtx_rot.SetFromMatrix(mtx);

	btTransform result;
	result.setOrigin(btVector3(mtx_position.x(), mtx_position.y(), mtx_position.z()));
	result.setRotation(btQuaternion(mtx_rot.x, mtx_rot.y, mtx_rot.z, mtx_rot.w));

	return result;
}