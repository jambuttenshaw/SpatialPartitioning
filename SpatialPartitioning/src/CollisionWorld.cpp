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
	mWorldBounds = worldBounds;

	// when the world is resized, the spacial partition needs completely reconstructed
	if (mSpatialPartition != nullptr)
	{
		mSpatialPartition->ClearAndResizeWorld(worldBounds);

		// re-insert all geometry in the collision world into the spacial partition
		for (const auto& [id, index] : mIDToIndex)
		{
			mSpatialPartition->Insert(id, mObjects[index]);
		}
	}
}

ColliderID CollisionWorld::AddAABB(const AABB& aabb)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(aabb)) && "AABB is outside of the world!");

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

void CollisionWorld::ResetObject(ColliderID id, const AABB& bounds)
{
	// make sure the id is valid
	assert(mIDToIndex.find(id) != mIDToIndex.end());
	
	// to modify the bounds of an object, it has to be removed from the spatial partition then re-inserted after modification
	if (mSpatialPartition != nullptr) mSpatialPartition->Delete(id, Get(id));
	GetNonConst(id) = bounds;
	if (mSpatialPartition != nullptr) mSpatialPartition->Insert(id, bounds);
}

void CollisionWorld::Translate(ColliderID id, const Vector2f& translation)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());

	// to modify the bounds of an object, it has to be removed from the spatial partition then re-inserted after modification
	if (mSpatialPartition != nullptr) mSpatialPartition->Delete(id, Get(id));
	GetNonConst(id).Translate(translation);
	if (mSpatialPartition != nullptr) mSpatialPartition->Insert(id, Get(id));
}

void CollisionWorld::SetPosition(ColliderID id, const Vector2f& position)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());

	// to modify the bounds of an object, it has to be removed from the spatial partition then re-inserted after modification
	if (mSpatialPartition != nullptr) mSpatialPartition->Delete(id, Get(id));
	GetNonConst(id).SetPosition(position);
	if (mSpatialPartition != nullptr) mSpatialPartition->Insert(id, Get(id));
}

void CollisionWorld::Resize(ColliderID id, const Vector2f& newSize)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());

	// to modify the bounds of an object, it has to be removed from the spatial partition then re-inserted after modification
	if (mSpatialPartition != nullptr) mSpatialPartition->Delete(id, Get(id));
	GetNonConst(id).Resize(newSize);
	if (mSpatialPartition != nullptr) mSpatialPartition->Insert(id, Get(id));
}


const AABB& CollisionWorld::Get(ColliderID id)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());
	// returns the bounds of the object with ID, that cannot be modified
	return mObjects[mIDToIndex[id]];
}

AABB& CollisionWorld::GetNonConst(ColliderID id)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());
	// returns the bounds of the object with ID, that can be modified
	// for use within this class only
	return mObjects[mIDToIndex[id]];
}


std::set<ColliderID> CollisionWorld::GetCollisions(ColliderID id)
{
	assert(mIDToIndex.find(id) != mIDToIndex.end());

	// if we do not have a spatial partition then we perform brute force method
	if (mSpatialPartition == nullptr) return GetCollisionsBruteForce(id);

	// get the bounds of the object
	const AABB& object = Get(id);

	// perform broad-phase collision detection using the spacial partition

	// create a container to hold all potential collisions
	// a set is used as that automatically guarantees that 
	std::set<ColliderID> potentialCollisions;
	mSpatialPartition->Retrieve(potentialCollisions, object);

	// a container of all confirmed collisions
	std::set<ColliderID> collisions;
	
	// perform narrow-phase collision detection
	for (auto potentialCollisionID : potentialCollisions)
	{
		// make sure were not comparing an object with itself
		if (potentialCollisionID != id)
		{
			// test if the two objects actually overlap
			const AABB& potentialCollision = Get(potentialCollisionID);
			if (object.Intersects(potentialCollision))
				// they do, a collision has been found
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

	// even though this is obvious from the test case,
	// its helpful to output it into the csv file
	//size_t AverageCollisionChecks = mIDToIndex.size();
	//TRACK_AVERAGE(AverageCollisionChecks);

	for (const auto& [colliderId, index] : mIDToIndex)
	{
		if (id == colliderId)
			continue;

		const AABB& otherBoundingBox = mObjects[index];

		if (boundingBox.Intersects(otherBoundingBox))
		{
			collisions.insert(colliderId);
		}
	}

	return collisions;
}

