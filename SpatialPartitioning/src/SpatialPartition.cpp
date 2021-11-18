#include "SpatialPartition.h"

#include "CollisionWorld.h"


void SpatialPartition::SetCollisionWorld(CollisionWorld* collisionWorld)
{
	mCollisionWorld = collisionWorld;
}

const AABB* SpatialPartition::GetAABBFromColliderID(ColliderID id)
{
	return mCollisionWorld->Get(id);
}