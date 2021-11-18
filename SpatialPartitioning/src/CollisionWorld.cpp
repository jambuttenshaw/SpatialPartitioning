#include "CollisionWorld.h"


#include <cassert>

ColliderID CollisionWorld::CreateAABB(const AABB& aabb)
{
	ColliderID newIndex = mObjects.size();

	mObjects.push_back(aabb);
	mSpacialPartition->Insert(newIndex);

	return newIndex;
}

void CollisionWorld::DeleteAABB(ColliderID id)
{
	assert(id < mObjects.size());

	mSpacialPartition->Delete(id);

	// THIS NEEDS FIXED! 
	// THIS WILL INVALIDATE MANY POINTERS IN THE SPATIAL PARTITION SYSTEM!!!
	assert(false); 
	mObjects.erase(mObjects.begin() + id);
}

void CollisionWorld::Translate(ColliderID id, const Vector2f& translation)
{
	assert(id < mObjects.size());

	mObjects[id].Translate(translation);
	// update object data inside spatial partition
}

void CollisionWorld::SetPosition(ColliderID id, const Vector2f& position)
{
	assert(id < mObjects.size());

	mObjects[id].SetPosition(position);
	// update object data inside spatial partition
}

void CollisionWorld::Resize(ColliderID id, const Vector2f& newSize)
{
	assert(id < mObjects.size());

	mObjects[id].Resize(newSize);
	// update object data inside spatial partition
}

AABB* CollisionWorld::Get(ColliderID id)
{
	assert(id < mObjects.size());
	return &mObjects[id];
}

std::vector<AABB*> CollisionWorld::GetCollisions(ColliderID id)
{
	assert(id < mObjects.size());

	const AABB& object = mObjects[id];
	
	// perform broad-phase collision detection using the spacial partition
	std::vector<ColliderID> potentialCollisions = mSpacialPartition->Retrieve(object);


	// perform narrow-phase collision detection
	std::vector<AABB*> collisions;
	// so that collisions will definitely only be resized once
	collisions.reserve(potentialCollisions.size());

	for (size_t i = 0; i < potentialCollisions.size(); i++)
	{
		AABB* potentialCollision = Get(potentialCollisions.at(i));

		if (object.Intersects(*potentialCollision))
			collisions[i] = potentialCollision;
	}
	collisions.shrink_to_fit();
	return collisions;
}
