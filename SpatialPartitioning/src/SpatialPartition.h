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
