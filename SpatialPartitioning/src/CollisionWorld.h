#pragma once

#include <vector>
#include <type_traits>

#include "Utilities/AABB.h"
#include "Utilities/Vector2.h"

#include "SpatialPartition.h"

using ColliderID = size_t;


class CollisionWorld
{
public:
	template<typename T>
	CollisionWorld()
	{
		CreateSpatialPartition<T>();
	}
	~CollisionWorld()
	{
		DeleteSpatialPartition();
	}
	template<typename T>
	void SetSpatialPartitioner()
	{
		DeleteSpatialPartition();
		CreateSpatialPartition<T>();
	}



	ColliderID CreateAABB(const AABB& aabb);
	void DeleteAABB(ColliderID id);
	
	void Translate(ColliderID id, const Vector2& translation);
	void SetPosition(ColliderID id, const Vector2& position);
	
	void Resize(ColliderID id, const Vector2& newSize);

	const AABB& Get(ColliderID id);

	std::vector<AABB*> GetCollisions(ColliderID id);

private:
	template<typename T>
	void CreateSpatialPartition()
	{
		static_assert(std::is_base_of<SpatialPartition::T>::value);
		mSpacialPartition = new T;
	}
	void DeleteSpatialPartition()
	{
		delete mSpacialPartition;
	}

private:
	std::vector<AABB> mObjects;
	SpatialPartition* mSpacialPartition = nullptr;
};
