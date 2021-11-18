#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>

#include "Utilities/Constants.h"
#include "Utilities/AABB.h"

#include "SpatialPartition.h"


class CollisionWorld
{
public:
	template <typename T>
	static CollisionWorld* Create(const AABB& worldBounds)
	{
		// the spatial partitioning system must inherit from SpatialPartition
		static_assert(std::is_base_of<SpatialPartition, T>::value);

		CollisionWorld* instance = new CollisionWorld{ worldBounds };
		instance->mSpacialPartition = new T(worldBounds);

		return instance;
	}

private:
	CollisionWorld(const AABB& worldBounds);
		
public:
	~CollisionWorld();


	ColliderID CreateAABB(const AABB& aabb);
	void DeleteAABB(ColliderID id);
	
	void Translate(ColliderID id, const Vector2f& translation);
	void SetPosition(ColliderID id, const Vector2f& position);
	
	void Resize(ColliderID id, const Vector2f& newSize);

	const AABB& Get(ColliderID id);

	std::vector<ColliderID> GetCollisions(ColliderID id);

private:
	// only allow AABB's to be directly modified from within this class
	AABB& GetNonConst(ColliderID id);

private:
	std::vector<AABB> mObjects;
	SpatialPartition* mSpacialPartition = nullptr;

	std::unordered_map<ColliderID, size_t> mIDToIndex;
	std::unordered_map<size_t, ColliderID> mIndexToID;
	ColliderID mNextID = ColliderID();

	AABB mWorldBounds;
};
