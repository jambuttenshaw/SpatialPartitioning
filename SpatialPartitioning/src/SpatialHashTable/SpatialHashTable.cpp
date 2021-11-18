#include "SpatialHashTable.h"


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
}

void SpatialHashTable::Delete(AABB* object)
{
}

std::vector<AABB*> SpatialHashTable::Retrieve(const AABB& bounds)
{

}


size_t SpatialHashTable::GetIndex(Vector2 position)
{
	// because the world is a finite size, we know there will be no collisions in this "hash" function
	// each cell in the grid can be given an integer ID, and this can be used as the index into the table
	Vector2 cell{ floorf(position.x / mCellSize), floorf(position.y / mCellSize) };
	return static_cast<size_t>(cell.x + (cell.y * mCellsX));
}
