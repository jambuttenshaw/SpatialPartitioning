#pragma once

#include <set>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <cassert>

#include "Utilities/Constants.h"
#include "Utilities/AABB.h"

#include "SpatialPartition.h"


class CollisionWorld
{
public:
	static CollisionWorld* Instance()
	{
		static CollisionWorld instance;
		return &instance;
	}

private:
	CollisionWorld() = default;
		
public:
	~CollisionWorld();

	// setup functions
	template<typename T>
	void SetSpatialPartitioner()
	{
		// the spatial partitioning system must inherit from SpatialPartition
		static_assert(std::is_base_of<SpatialPartition, T>::value);
		// the world size must be set before creating a spatial partition
		assert((mWorldBounds.Size() != Vector2f::Zero) && "Initialize world size before partitioning!");

		// check if we already have a spatial partition
		if (mSpatialPartition != nullptr)
		{
			// delete the old spatial partition
			delete mSpatialPartition;
		}
		mSpatialPartition = new T(mWorldBounds);
		
		// insert all geometry in the collision world into the new partition
		for (const auto& [id, index] : mIDToIndex)
		{
			mSpatialPartition->Insert(id, mObjects[index]);
		}
	}
	SpatialPartition* GetSpatialPartition() const { return mSpatialPartition; }

	void SetWorldBounds(const AABB& worldBounds);

	// adding and removing colliders
	ColliderID AddAABB(const AABB& aabb);
	void DeleteAABB(ColliderID id);

	// clear all collision geometry from the world
	void Clear();
	

	// modifying existing colliders
	void ResetObject(ColliderID id, const AABB& bounds);

	void Translate(ColliderID id, const Vector2f& translation);
	void SetPosition(ColliderID id, const Vector2f& position);
	
	void Resize(ColliderID id, const Vector2f& newSize);

	// getters
	const AABB& Get(ColliderID id);
	inline const AABB& GetWorldBounds() const { return mWorldBounds; }

	// the important function: find every collider that overlaps with the input collider
	std::set<ColliderID> GetCollisions(ColliderID id);
	// find all collisions with the input collider without using the spatial partition
	std::set<ColliderID> GetCollisionsBruteForce(ColliderID id);

private:
	// only allow AABB's to be directly modified from within this class
	AABB& GetNonConst(ColliderID id);

private:
	// all objects in the world
	std::vector<AABB> mObjects;
	// the spatial partition object used to speed up detecting collisions between objects
	SpatialPartition* mSpatialPartition = nullptr;

	// for improved data locality in mObjects
	// maps between an ID and the index in the objects array
	// as it is an unordered map, we still have constant time access to objects but with improved use of the cache
	std::unordered_map<ColliderID, size_t> mIDToIndex;
	std::unordered_map<size_t, ColliderID> mIndexToID;

	// begin with the first colliderID
	ColliderID mNextID = ColliderID();

	// the size of the world
	AABB mWorldBounds;
};
