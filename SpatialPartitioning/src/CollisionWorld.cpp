#include "CollisionWorld.h"


#include <cassert>

ColliderID CollisionWorld::CreateAABB(const AABB& aabb)
{
}

void CollisionWorld::DeleteAABB(ColliderID id)
{
}

void CollisionWorld::Translate(ColliderID id, const Vector2& translation)
{
	assert(id < mObjects.size());

	mObjects[id].Translate(translation);
	// update object data inside spatial partition
}

void CollisionWorld::SetPosition(ColliderID id, const Vector2& position)
{
	assert(id < mObjects.size());

	mObjects[id].SetPosition(position);
	// update object data inside spatial partition
}

void CollisionWorld::Resize(ColliderID id, const Vector2& newSize)
{
	assert(id < mObjects.size());

	mObjects[id].Resize(newSize);
	// update object data inside spatial partition
}

const AABB& CollisionWorld::Get(ColliderID id)
{
	assert(id < mObjects.size());
	return mObjects[id];
}

std::vector<AABB*> CollisionWorld::GetCollisions(ColliderID id)
{
	assert(id < mObjects.size());

	const AABB& object = mObjects[id];
	
	// perform broad-phase collision detection using the spacial partition
	std::vector<AABB*> potentialCollisions = mSpacialPartition->GetPotentialCollisions(object);


	// perform narrow-phase collision detection
	std::vector<AABB*> collisions;
	// so that collisions will definitely only be resized once
	collisions.reserve(potentialCollisions.size());

	for (size_t i = 0; i < potentialCollisions.size(); i++)
	{
		AABB* potentialCollision = potentialCollisions.at(i);

		if (object.Intersects(*potentialCollision))
			collisions[i] = potentialCollision;
	}
	collisions.shrink_to_fit();
	return collisions;
}
