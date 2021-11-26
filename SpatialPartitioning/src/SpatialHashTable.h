#pragma once

#include "SpatialPartition.h"

#include <vector>
#include <forward_list>

class SpatialHashTable : public SpatialPartition
{
public:
	SpatialHashTable(const AABB& worldBounds);
	virtual ~SpatialHashTable() = default;

	virtual void Insert(ColliderID object, const AABB& bounds) override;
	virtual void Delete(ColliderID object, const AABB& bounds) override;

	virtual void Clear() override;

	virtual void Retrieve(std::set<ColliderID>& out, const AABB& bounds) override;

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
	size_t mCellsX = 0, mCellsY = 0;
};
