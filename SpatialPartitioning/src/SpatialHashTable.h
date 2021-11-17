#pragma once

#include "SpatialPartition.h"

#include <list>

class SpatialHashTable : public SpatialPartition
{
public:
	SpatialHashTable() = default;
	virtual ~SpatialHashTable() = default;

	virtual std::vector<AABB*> GetPotentialCollisions(const AABB& object) override;

	virtual void Insert(AABB* object) override;
	virtual void Delete(AABB* object) override;

	virtual void UpdateColliderInPartition(AABB* object) override;

private:

	Vector2 GetGridCell(Vector2 position);
	size_t GetIndex(Vector2 gridCell);

	bool CheckTableRequiresResize();
	void ResizeTable();

private:
	// the hash table data structure
	// a container to contain the buckets
	std::vector<std::list<AABB*>> mTable;

	// the number of buckets that have at least one collider in them
	size_t mCount = 0;
	// the total number of colliders in the spatial partition
	// NOTE: a collider can appear in multiple buckets, which his value does not take into account
	size_t mColliderCount = 0;

	// the threshold load factor at which the table should be resized
	// eg: 0.8 means when the table is 80% full, make it bigger to reduce chance of collisions
	const float mThresholdLoadFactor = 0.8f;
};
