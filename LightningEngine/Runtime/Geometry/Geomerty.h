#pragma once

#include "AABB.h"
#include "Sphere.h"

namespace LightningEngine
{
	enum BoundingVolumeType
	{
		kBoundingVolumeTypeNone,
		kBoundingVolumeTypeAABB,
		kBoundingVolumeTypeSphere,
		kBoundingVolumeTypeCapsule,
		kBoundingVolumeTypeCount
	};
}