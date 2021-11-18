#include "SpatialHashTable.h"

#include <cassert>

SpatialHashTable::SpatialHashTable(const AABB& worldBounds)
	: SpatialPartition(worldBounds)
{
	// number of cells on the x axis
	mCellsX = static_cast<unsigned int>(floorf(mWorldBounds.Size().x / mCellSize));
	mCellsY = static_cast<unsigned int>(floorf(mWorldBounds.Size().y / mCellSize));

	// resize the table to match the number of cells in the grid
	// as the world is a fixed size, we can design our hash function such that collisions are impossible
	unsigned int cellCount = mCellsX * mCellsY;
	mTable.resize(cellCount);
}

void SpatialHashTable::Insert(AABB* object)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(object->TopLeft()) &&
		mWorldBounds.Contains(object->TopLeft() + object->Size()))
		&& "AABB is outside of the world!");


	// get the grid index of the topleft and bottom right of the object
	Vector2 topLeftCell = GetCell(object->TopLeft());
	Vector2 bottomRightCell = GetCell(object->TopLeft() + object->Size());

	// perform some check to make sure this AABB has not already been partitioned
	if (BucketContainsAABB(object, GetIndex(topLeftCell))) return;


	// iterate through every cell the object is in
	for (int y = topLeftCell.y; y < bottomRightCell.y - topLeftCell.y; y++)
	{
		for (int x = topLeftCell.x; x < bottomRightCell.x - topLeftCell.x; x++)
		{
			// insert this object into this cell
			mTable[GetIndex({ x, y })].push_front(object);
		}
	}
}

void SpatialHashTable::Delete(AABB* object)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(object->TopLeft()) &&
		mWorldBounds.Contains(object->TopLeft() + object->Size()))
		&& "AABB is outside of the world!");


	// get the grid index of the topleft and bottom right of the object
	Vector2 topLeftCell = GetCell(object->TopLeft());
	Vector2 bottomRightCell = GetCell(object->TopLeft() + object->Size());


	// iterate through every cell the object is in
	for (int y = topLeftCell.y; y < bottomRightCell.y - topLeftCell.y; y++)
	{
		for (int x = topLeftCell.x; x < bottomRightCell.x - topLeftCell.x; x++)
		{
			// remove this object from this cell
			mTable[GetIndex({ x, y })].remove(object);
		}
	}
}

std::vector<AABB*> SpatialHashTable::Retrieve(const AABB& bounds)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(bounds.TopLeft()) &&
		mWorldBounds.Contains(bounds.TopLeft() + bounds.Size()))
		&& "AABB is outside of the world!");

	// get the grid index of the topleft and bottom right of the object
	Vector2 topLeftCell = GetCell(bounds.TopLeft());
	Vector2 bottomRightCell = GetCell(bounds.TopLeft() + bounds.Size());

	std::vector<AABB*> results;
	// iterate through every cell the object is in
	for (int y = topLeftCell.y; y < bottomRightCell.y - topLeftCell.y; y++)
	{
		for (int x = topLeftCell.x; x < bottomRightCell.x - topLeftCell.x; x++)
		{
			// add everything in this cell to the potential collisions
			auto& bucket = mTable[GetIndex({ x, y })];
			results.insert(results.end(), bucket.begin(), bucket.end());
		}
	}

	return results;
}


Vector2 SpatialHashTable::GetCell(Vector2 position)
{
	return Vector2{ floorf(position.x / mCellSize), floorf(position.y / mCellSize) };
}

size_t SpatialHashTable::GetIndex(Vector2 cell)
{
	// because the world is a finite size, we know there will be no collisions in this "hash" function
	// each cell in the grid can be given an integer ID, and this can be used as the index into the table
	return static_cast<size_t>(cell.x + (cell.y * mCellsX));
}

bool SpatialHashTable::BucketContainsAABB(AABB* aabb, size_t bucketIndex)
{
	auto& bucket = mTable[bucketIndex];
	for (const auto& i : bucket)
	{
		if (i == aabb) return true;
	}
	return false;
}
