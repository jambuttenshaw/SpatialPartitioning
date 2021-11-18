#include "CollisionWorld.h"


#include <cassert>

CollisionWorld::CollisionWorld(const AABB& worldBounds)
	: mWorldBounds(worldBounds)
{
}
CollisionWorld::~CollisionWorld()
{
	delete mSpacialPartition;
}

ColliderID CollisionWorld::CreateAABB(const AABB& aabb)
{
	// TODO: add check to make sure we havent reached max colliders yet

	// get the next available ID for a collider
	ColliderID newID = mNextID;

	// add a mapping from the id to the colliders position in the vector
	mIDToIndex.insert({ newID, mObjects.size() });
	// add a mapping from the position in the vector to the colliders id
	mIndexToID.insert({ mObjects.size(), newID });

	// add the object to the vector
	mObjects.push_back(aabb);

	// insert this object into the spacial partition
	mSpacialPartition->Insert(newID);

	// increment the available id
	mNextID++;

	return newID;
}

void CollisionWorld::DeleteAABB(ColliderID id)
{
	// make sure this ID exists
	assert(mIDToIndex.find(id) != mIDToIndex.end());

	// tell the spatial partitioning system the object has been deleted
	mSpacialPartition->Delete(id);

	// to retain density in the vector, we will move the last element in the vector
	// into the space of the object were about to delete
	size_t lastIndex = mObjects.size() - 1;
	ColliderID lastObjectID = mIndexToID[lastIndex];

	size_t indexToDelete = mIDToIndex[id];

	// update the data in the vector
	mObjects[indexToDelete] = mObjects[lastIndex];
	// update the mappings
	mIDToIndex[lastObjectID] = indexToDelete;
	mIndexToID[indexToDelete] = lastObjectID;

	// erase the mappings we no longer need
	mIDToIndex.erase(id);
	mIndexToID.erase(lastIndex);

	// remove the last item from the vector
	mObjects.erase(mObjects.begin() + lastIndex);
}

void CollisionWorld::Translate(ColliderID id, const Vector2f& translation)
{
	GetNonConst(id).Translate(translation);
	// update object data inside spatial partition
	assert(false);
}

void CollisionWorld::SetPosition(ColliderID id, const Vector2f& position)
{
	GetNonConst(id).SetPosition(position);
	// update object data inside spatial partition
	assert(false);
}

void CollisionWorld::Resize(ColliderID id, const Vector2f& newSize)
{
	assert(id < mObjects.size());

	GetNonConst(id).Resize(newSize);
	// update object data inside spatial partition
	assert(false);
}


const AABB& CollisionWorld::Get(ColliderID id)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());
	return mObjects[mIDToIndex[id]];
}

AABB& CollisionWorld::GetNonConst(ColliderID id)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());
	return mObjects[mIDToIndex[id]];
}


std::vector<ColliderID> CollisionWorld::GetCollisions(ColliderID id)
{
	const AABB& object = Get(id);
	
	// perform broad-phase collision detection using the spacial partition
	std::vector<ColliderID> potentialCollisions = mSpacialPartition->Retrieve(object);


	// perform narrow-phase collision detection
	std::vector<ColliderID> collisions;
	// so that collisions will definitely only be resized once
	collisions.reserve(potentialCollisions.size());

	for (auto potentialCollisionID : potentialCollisions)
	{
		const AABB& potentialCollision = Get(potentialCollisionID);

		if (object.Intersects(potentialCollision))
			collisions.push_back(potentialCollisionID);
	}
	collisions.shrink_to_fit();
	return collisions;
}

