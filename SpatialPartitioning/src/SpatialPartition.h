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
	* GetAABBFromColliderID is a constant-time function, do not fear.
	*/
	virtual void Insert(ColliderID object) = 0;
	virtual void Delete(ColliderID object) = 0;

	virtual std::vector<ColliderID> Retrieve(const AABB& bounds) = 0;


	// set up from collision world
	void SetCollisionWorld(CollisionWorld* collisionWorld);

protected:
	const AABB* GetAABBFromColliderID(ColliderID id);

protected:
	AABB mWorldBounds;

private:
	CollisionWorld* mCollisionWorld = nullptr;
};
