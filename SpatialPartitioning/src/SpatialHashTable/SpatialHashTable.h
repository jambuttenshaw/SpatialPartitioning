#pragma once

#include "SpatialPartition.h"

#include <forward_list>

class SpatialHashTable : public SpatialPartition
{
public:
	SpatialHashTable(const AABB& worldBounds);
	virtual ~SpatialHashTable() = default;

	virtual void Insert(ColliderID object) override;
	virtual void Delete(ColliderID object) override;

	virtual std::vector<ColliderID> Retrieve(const AABB& bounds) override;

private:

	Vector2i GetCell(Vector2f position);
	size_t GetIndex(Vector2i cell);

	bool BucketContainsCollider(ColliderID aabb, size_t bucketIndex);

private:
	// the hash table data structure
	// a container to contain the buckets
	std::vector<std::forward_list<ColliderID>> mTable;

	// the size of each cell in the grid
	// all cells in the grid are square, so it need only be represented by a single float
	const float mCellSize = 4.0f;
	unsigned int mCellsX = 0, mCellsY = 0;

	// the number of buckets that have at least one collider in them
	size_t mCount = 0;
	// the total number of colliders in the spatial partition
	// NOTE: a collider can appear in multiple buckets, which his value does not take into account
	size_t mColliderCount = 0;
};
