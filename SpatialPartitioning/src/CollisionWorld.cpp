#include "CollisionWorld.h"

#include "Utilities/Instrumentor.h"


CollisionWorld::~CollisionWorld()
{
	// its possible the spatital partition may never be set
	if (mSpatialPartition != nullptr)
		delete mSpatialPartition;
}

void CollisionWorld::SetWorldBounds(const AABB& worldBounds)
{
	assert((mSpatialPartition == nullptr) && "Cannot modify world size after creating spatial partition!");
	mWorldBounds = worldBounds;
}

ColliderID CollisionWorld::AddAABB(const AABB& aabb)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(aabb)) && "AABB is outside of the world!");

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
	if (mSpatialPartition != nullptr)
		mSpatialPartition->Insert(newID, aabb);

	// increment the available id
	mNextID++;

	return newID;
}

void CollisionWorld::DeleteAABB(ColliderID id)
{
	// make sure this ID exists
	assert(mIDToIndex.find(id) != mIDToIndex.end());

	// tell the spatial partitioning system the object has been deleted
	if (mSpatialPartition != nullptr)
		mSpatialPartition->Delete(id, Get(id));

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

void CollisionWorld::Clear()
{
	// Empty all collision geometry in the world and restart collider ID's
	if (mSpatialPartition != nullptr)
		mSpatialPartition->Clear();

	mObjects.clear();
	mIDToIndex.clear();
	mIndexToID.clear();

	mNextID = ColliderID();
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


std::set<ColliderID> CollisionWorld::GetCollisions(ColliderID id)
{
	if (mSpatialPartition == nullptr) return GetCollisionsBruteForce(id);


	const AABB& object = Get(id);
	
	// perform broad-phase collision detection using the spacial partition
	std::set<ColliderID> potentialCollisions;
	mSpatialPartition->Retrieve(potentialCollisions, object);

	size_t averageCollisionChecks = potentialCollisions.size();
	TRACK_AVERAGE(averageCollisionChecks);

	// perform narrow-phase collision detection
	std::set<ColliderID> collisions;

	for (auto potentialCollisionID : potentialCollisions)
	{
		// make sure were not comparing an object with itself
		if (potentialCollisionID != id)
		{
			const AABB& potentialCollision = Get(potentialCollisionID);
			if (object.Intersects(potentialCollision))
				collisions.insert(potentialCollisionID);
		}
	}

	return collisions;
}

/// <summary>
///  No broad-phase collision detection, just test for intersection between each pair of colliders
/// </summary>
std::set<ColliderID> CollisionWorld::GetCollisionsBruteForce(ColliderID id)
{
	std::set<ColliderID> collisions;

	const AABB& boundingBox = Get(id);
	for (const auto& collider : mIDToIndex)
	{
		if (id == collider.first)
			continue;

		const AABB& otherBoundingBox = mObjects[collider.second];

		if (boundingBox.Intersects(otherBoundingBox))
		{
			collisions.insert(collider.first);
		}
	}

	return collisions;
}

