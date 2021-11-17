#pragma once

#include "Utilities/AABB.h"
#include <vector>


class SpatialPartition
{
public:
	SpatialPartition() = default;
	virtual ~SpatialPartition() = default;

	virtual std::vector<AABB*> GetPotentialCollisions(const AABB& object) = 0;

	virtual void Insert(AABB* object) = 0;
	virtual void Delete(AABB* object) = 0;

	virtual void UpdateColliderInPartition(AABB* object) = 0;
};
