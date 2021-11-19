#pragma once

#include "Utilities/Constants.h"
#include "Utilities/AABB.h"

#include <vector>
#include <memory>

// forward-declare to avoid circular dependencies
class CollisionWorld;


class SpatialPartition
{
public:
	SpatialPartition(const AABB& worldBounds) : mWorldBounds(worldBounds) {}
	virtual ~SpatialPartition() = default;

	/*
	* IMPORTANT!
	* NOWHERE inside a spatial partitioning system should pointers to AABB's be stored as members
	* when an AABB is deleted from the collision world, this will likely invalidate this pointer
	* and you will have no way of knowing this has happened!
	* 
	* Use CollisionWorld::Get(ColliderID) to get the AABB of an object given its ID
	*/
	virtual void Insert(ColliderID object, const AABB& objectBounds) = 0;
	virtual void Delete(ColliderID object, const AABB& objectBounds) = 0;

	// Empty everything stored in the spatial partition
	virtual void Clear() = 0;

	// Populate the vector 'out' with all ColliderID's that could potentitall collide with the input bounds 'bounds'
	virtual void Retrieve(std::vector<ColliderID>& out, const AABB& bounds) = 0;

protected:
	AABB mWorldBounds;
};
