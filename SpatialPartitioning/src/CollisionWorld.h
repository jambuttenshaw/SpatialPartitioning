#pragma once

#include <vector>
#include <type_traits>

#include "Utilities/Constants.h"
#include "Utilities/AABB.h"

#include "SpatialPartition.h"


class CollisionWorld
{
public:
	template<typename T>
	CollisionWorld(const AABB& worldBounds)
		: mWorldBounds(worldBounds)
	{
		// the spatial partitioning system must inherit from SpatialPartition
		static_assert(std::is_base_of<SpatialPartition, T>::value);

		mSpacialPartition = new T(mWorldBounds);
		mSpacialPartition->SetCollisionWorld(this);
	}
	~CollisionWorld()
	{
		delete mSpacialPartition;
	}


	ColliderID CreateAABB(const AABB& aabb);
	void DeleteAABB(ColliderID id);
	
	void Translate(ColliderID id, const Vector2f& translation);
	void SetPosition(ColliderID id, const Vector2f& position);
	
	void Resize(ColliderID id, const Vector2f& newSize);

	AABB* Get(ColliderID id);

	std::vector<AABB*> GetCollisions(ColliderID id);


private:
	std::vector<AABB> mObjects;
	SpatialPartition* mSpacialPartition = nullptr;

	AABB mWorldBounds;
};
