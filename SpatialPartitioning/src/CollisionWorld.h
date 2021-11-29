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

		if (mSpatialPartition == nullptr)
		{
			mSpatialPartition = new T(mWorldBounds);
		}
		else
		{
			// delete the old spatial partition
			delete mSpatialPartition;
			// create new one
			mSpatialPartition = new T(mWorldBounds);
		}
		
		// insert all geometry in the collision world into the new partition
		for (const auto& collider : mIDToIndex)
		{
			mSpatialPartition->Insert(collider.first, mObjects[collider.second]);
		}
	}
	void SetWorldBounds(const AABB& worldBounds);

	ColliderID AddAABB(const AABB& aabb);
	void DeleteAABB(ColliderID id);

	void Clear();
	
	void Translate(ColliderID id, const Vector2f& translation);
	void SetPosition(ColliderID id, const Vector2f& position);
	
	void Resize(ColliderID id, const Vector2f& newSize);

	const AABB& Get(ColliderID id);
	inline const AABB& GetWorldBounds() const { return mWorldBounds; }

	std::set<ColliderID> GetCollisions(ColliderID id);
	std::set<ColliderID> GetCollisionsBruteForce(ColliderID id);

private:
	// only allow AABB's to be directly modified from within this class
	AABB& GetNonConst(ColliderID id);

private:
	std::vector<AABB> mObjects;
	SpatialPartition* mSpatialPartition = nullptr;

	std::unordered_map<ColliderID, size_t> mIDToIndex;
	std::unordered_map<size_t, ColliderID> mIndexToID;
	ColliderID mNextID = ColliderID();

	AABB mWorldBounds;
};
