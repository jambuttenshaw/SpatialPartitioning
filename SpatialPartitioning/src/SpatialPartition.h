#pragma once

#include "Utilities/Constants.h"
#include "Utilities/AABB.h"

#include <set>
#include <memory>

// forward-declare to avoid circular dependencies
class CollisionWorld;


class SpatialPartition
{
public:
	SpatialPartition(const AABB& worldBounds) : mWorldBounds(worldBounds) {}
	virtual ~SpatialPartition() = default;

	virtual void ClearAndResizeWorld(const AABB& worldBounds) = 0;

	/*
	* IMPORTANT!
	* A spatial partitioning system should NOT store pointers to AABB's.
	* when an AABB is deleted from the collision world, this re-orders some AABB's in memory to preserve data locality.
	* This will invalidate the pointers to the AABB
	* and you will have no way of knowing this has happened!
	* 
	* Use CollisionWorld::Get(ColliderID) to get the AABB of an object given its ID
	*/
	virtual void Insert(ColliderID object, const AABB& objectBounds) = 0;
	virtual void Delete(ColliderID object, const AABB& objectBounds) = 0;

	// Empty everything stored in the spatial partition
	virtual void Clear() = 0;

	// Populate the vector 'out' with all ColliderID's that could potentitall collide with the input bounds 'bounds'
	virtual void Retrieve(std::set<ColliderID>& out, const AABB& bounds) = 0;

protected:
	AABB mWorldBounds;
};
