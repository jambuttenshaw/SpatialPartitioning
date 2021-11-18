#pragma once

#include "Utilities/AABB.h"

#include <vector>
#include <memory>


class SpatialPartition
{
public:
	SpatialPartition(const AABB& worldBounds) : mWorldBounds(worldBounds) {}
	virtual ~SpatialPartition() = default;

	virtual void Insert(AABB* object) = 0;
	virtual void Delete(AABB* object) = 0;

	virtual std::vector<AABB*> Retrieve(const AABB& bounds) = 0;

protected:
	AABB mWorldBounds;
};
