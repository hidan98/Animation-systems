#ifndef _CCD_H

#include <animation/skeleton.h>
#include <graphics/skinned_mesh_instance.h>

bool CalculateCCD(
	gef::SkeletonPose& pose,
	const gef::SkinnedMeshInstance& animatedModel,
	const gef::Vector4& destPoint,
	const std::vector<int>& boneIndices);

float getDistance(gef::Vector4 endEffector, gef::Vector4 destination);

#endif // !_CCD_H
