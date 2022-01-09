#include "SpatialHashTable.h"

#include "Utilities/Instrumentor.h"
#include <cassert>

SpatialHashTable::SpatialHashTable(const AABB& worldBounds)
	: SpatialPartition(worldBounds)
{
	SetupTable();
}

void SpatialHashTable::ClearAndResizeWorld(const AABB& bounds)
{
	Clear();
	mWorldBounds = bounds;
	SetupTable();
}

void SpatialHashTable::Insert(ColliderID object, const AABB& bounds)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(bounds)) && "AABB is outside of the world!");


	// get the grid index of the topleft and bottom right of the object
	Vector2i topLeftCell = GetCell(bounds.TopLeft());
	Vector2i bottomRightCell = GetCell(bounds.TopLeft() + bounds.Size());

	// perform some check to make sure this AABB has not already been partitioned
	if (BucketContainsCollider(object, GetIndex(topLeftCell))) return;


	// iterate through every cell the object is in
	for (int y = topLeftCell.y; y <= bottomRightCell.y; y++)
	{
		for (int x = topLeftCell.x; x <= bottomRightCell.x; x++)
		{
			// insert this object into this cell
			mTable[GetIndex({ x, y })].emplace_front(object);
		}
	}

	mObjectCount++;
}

void SpatialHashTable::Delete(ColliderID object, const AABB& bounds)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(bounds.TopLeft()) &&
		mWorldBounds.Contains(bounds.TopLeft() + bounds.Size()))
		&& "AABB is outside of the world!");


	// get the grid index of the topleft and bottom right of the object
	Vector2i topLeftCell = GetCell(bounds.TopLeft());
	Vector2i bottomRightCell = GetCell(bounds.TopLeft() + bounds.Size());


	// iterate through every cell the object is in
	for (int y = topLeftCell.y; y <= bottomRightCell.y; y++)
	{
		for (int x = topLeftCell.x; x <= bottomRightCell.x; x++)
		{
			// remove this object from this cell
			mTable[GetIndex({ x, y })].remove(object);
		}
	}

	mObjectCount--;
}

void SpatialHashTable::Clear()
{
	// empty the table
	mTable.clear();
	// then re-initialize the table back to its empty state
	mTable.resize(mCellsX * mCellsY);

	mObjectCount = 0;
}

void SpatialHashTable::Retrieve(std::set<ColliderID>& out, const AABB& bounds)
{
	// make sure the AABB is entirely inside the world
	// if both corners of the AABB are inside the world, the entire AABB must be inside the world
	assert((mWorldBounds.Contains(bounds.TopLeft()) &&
		mWorldBounds.Contains(bounds.TopLeft() + bounds.Size()))
		&& "AABB is outside of the world!");

	// get the grid index of the topleft and bottom right of the object
	Vector2i topLeftCell = GetCell(bounds.TopLeft());
	Vector2i bottomRightCell = GetCell(bounds.TopLeft() + bounds.Size());

	// iterate through every cell the object is in
	for (int y = topLeftCell.y; y <= bottomRightCell.y; y++)
	{
		for (int x = topLeftCell.x; x <= bottomRightCell.x; x++)
		{
			// add everything in this cell to the potential collisions
			auto& bucket = mTable[GetIndex({ x, y })];
			out.insert(bucket.begin(), bucket.end());
		}
	}
}

void SpatialHashTable::SetCellSize(float cellSize)
{
	assert((mObjectCount == 0) && "Cannot adjust cell size while spatial partition is not empty!");

	mCellSize = cellSize;

	// reset the table size
	SetupTable();
}


void SpatialHashTable::SetupTable()
{
	// number of cells on the x axis
	mCellsX = static_cast<size_t>(ceilf(mWorldBounds.Size().x / mCellSize));
	mCellsY = static_cast<size_t>(ceilf(mWorldBounds.Size().y / mCellSize));

	// resize the table to match the number of cells in the grid
	// as the world is a fixed size, we can design our hash function such that collisions are impossible
	size_t cellCount = mCellsX * mCellsY;
	mTable.resize(cellCount);
}

Vector2i SpatialHashTable::GetCell(Vector2f position)
{
	// first we must move the topleft of the world to 0, 0
	position -= mWorldBounds.TopLeft();
	// then divide by the cell size so each cell has integer coordinates from 0-num cells on each axis
	return Vector2i{ static_cast<int>(floorf(position.x / mCellSize)),
					 static_cast<int>(floorf(position.y / mCellSize)) };
}

size_t SpatialHashTable::GetIndex(Vector2i cell)
{
	// because the world is a finite size, we know there will be no collisions in this "hash" function
	// each cell in the grid can be given an integer ID, and this can be used as the index into the table
	return static_cast<size_t>(cell.x) + (static_cast<size_t>(cell.y) * mCellsX);
}

bool SpatialHashTable::BucketContainsCollider(ColliderID collider, size_t bucketIndex)
{
	// check if a collider is in a bucket
	auto& bucket = mTable[bucketIndex];
	for (const auto& i : bucket)
	{
		if (i == collider) return true;
	}
	return false;
}
