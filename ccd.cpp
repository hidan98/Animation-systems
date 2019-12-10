#include "ccd.h"
#include <algorithm>
#include <system/debug_log.h>
#include <maths/math_utils.h>
#include <math.h>
#include <maths\vector4.h>
bool CalculateCCD(
	gef::SkeletonPose& pose,
	const gef::SkinnedMeshInstance& animatedModel,
	const gef::Vector4& destPoint,
	const std::vector<int>& boneIndices)
{
	std::vector<gef::Matrix44> global_pose;
	std::vector<gef::Matrix44> local_pose;
	local_pose.resize(pose.skeleton()->joint_count());
	global_pose = pose.global_pose();


	for (int i = 0; i < pose.skeleton()->joint_count(); ++i)
		local_pose[i] = pose.local_pose()[i].GetMatrix();

	//obtain the inverse of the animated model's transform
	gef::Matrix44 worldToModelTransform;
	worldToModelTransform.Inverse(animatedModel.transform());

	//...and use it to move the destination point from world space to model space
	gef::Vector4 destPointModelSpace = destPoint.Transform(worldToModelTransform);

	// Get the end effectors position
	gef::Vector4 endEffectorPos = global_pose[boneIndices.back()].GetTranslation();

	// FIXME: REPLACE THE LINE BELOW
	//calculate the distance between the end effector's position and the destination position
	float distance;// = getDistance(endEffectorPos, destPointModelSpace);

	distance = (endEffectorPos - destPointModelSpace).Length();

	float epsilon = 0.001f;
	//max number of iterations is used to stop CCD if there is no solution
	int maxIterations = 200;

	//perform the CCD algorithm if all the following conditions are valid
	/*
	- if the distance between the end effector point is greater than epsilon
	- we have not reached the maximum number of iterations
	*/
	while ((distance > epsilon) && (maxIterations > 0))
	{
		// FIXME: IMPLEMENT THE REVERSE ORDER FOR LOOP TO SOLVE THE IK CHAIN
		// USING THE ALGORITHM IN THE LECTURE NOTES

		if (boneIndices.size() >= 2)
		{
			//loop backwords
			for (size_t it = boneIndices.size() - 1; it--;)
			{
				int bonePos = boneIndices[it];

				//current bone trans
				gef::Vector4 currentBone = global_pose[bonePos].GetTranslation();
				
				//create a directional vector from the current bone to the end effector 
				gef::Vector4 eb;
				eb = endEffectorPos - currentBone;// endEffectorPos;
				eb.Normalise();

				//create a diretional vector fromt he current bone to the destination 
				gef::Vector4 db;
				db = destPointModelSpace - currentBone;// -destPointModelSpace;
				db.Normalise();


				float cosTheta = eb.DotProduct(db);
				if (cosTheta <= -1)
					cosTheta = -1;
				else if (cosTheta >= 1)
					cosTheta = 1;

				float angle = acosf(cosTheta);

				if (std::fabs(angle) < 0.1f)
					continue;

				//calculate the axis of rotation
				gef::Vector4 axis;
				axis = eb.CrossProduct(db);
				axis.Normalise();

				//calculate the bone rotation angle
				float sin_ = sin(angle * 0.5f);
				gef::Quaternion rotation(sin_ * axis.x(), sin_ * axis.y(), sin_ * axis.z(), cos(angle * 0.5f));

				gef::Matrix44 current;
				current = global_pose[bonePos];

				gef::Quaternion currentBoneRot;
				currentBoneRot.SetFromMatrix(global_pose[bonePos]);

				gef::Quaternion newBoneRot;

				newBoneRot = currentBoneRot * rotation;
				newBoneRot.Normalise();

				gef::Matrix44 newBoneTrans;
				newBoneTrans.Rotation(newBoneRot);
				newBoneTrans.SetTranslation(currentBone);
				global_pose[bonePos] = newBoneTrans;

				//loop through the bone indices starting at its current position +1 to update all of its children
				for (int i = it + 1; i < boneIndices.size(); i++)
				{
					int pos = boneIndices[i];
					gef::Matrix44 local_child;

					gef::Matrix44 inver;// = local_pose[pos];
					inver.Inverse(current);
					local_child = global_pose[pos] * inver;

					current = global_pose[pos];

					gef::Matrix44 worldChild;
					worldChild = local_child* newBoneTrans;
					
					newBoneTrans = worldChild;
					global_pose[pos] = worldChild;
					

				}
				//get new positoin of end effector
				endEffectorPos = global_pose[boneIndices.back()].GetTranslation();

				//calculate the new distance
				distance = (endEffectorPos - destPointModelSpace).Length();

			}

		}

		//if a solution has not been reached, decrement the iterations counter
		--maxIterations;
	}


	//
	// This remain part of the function updates the gef::SkeletonPose with the newly calculate bone
	// transforms.
	// The gef::SkeletonPose interface wasn't originally designed to be updated in this way
	// You may wish to consider altering the interface to this class to remove redundant calculations

	// calculate new local pose of bones in IK chain
	for (size_t i = 0; i < boneIndices.size(); ++i)
	{
		int boneNum = boneIndices[i];

		const gef::Joint& joint = pose.skeleton()->joint(boneNum);
		if (joint.parent == -1)
		{
			local_pose[boneNum] = global_pose[boneNum];
		}
		else
		{
			gef::Matrix44 parentInv;
			parentInv.Inverse(global_pose[joint.parent]);
			local_pose[boneNum] = global_pose[boneNum] * parentInv;
		}
	}

	// recalculate global pose based on new local pose
	for (int i = 0; i < pose.skeleton()->joint_count(); ++i)
	{
		const gef::Joint& joint = pose.skeleton()->joint(i);
		if (joint.parent == -1)
			global_pose[i] = local_pose[i];
		else
			global_pose[i] = local_pose[i] * global_pose[joint.parent];
	}

	// update skeleton pose data structure
	pose.CalculateLocalPose(global_pose);
	pose.CalculateGlobalPose();


	if (maxIterations <= 0)
	{
		//if the iterations counter reaches 0 (or less), then a CCD solution was not found
		return false;
	}
	else
	{

		//if a solution was found, return true
		return true;
	}

}


