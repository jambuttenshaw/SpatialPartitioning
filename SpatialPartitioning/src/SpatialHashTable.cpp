#include "SpatialHashTable.h"

std::vector<AABB*> SpatialHashTable::GetPotentialCollisions(const AABB& object)
{
}

void SpatialHashTable::Insert(AABB* object)
{
}

void SpatialHashTable::Delete(AABB* object)
{
}

void SpatialHashTable::UpdateColliderInPartition(AABB* object)
{
}

Vector2 SpatialHashTable::GetGridCell(Vector2 position)
{
}

size_t SpatialHashTable::GetIndex(Vector2 gridCell)
{
}

bool SpatialHashTable::CheckTableRequiresResize()
{
	float loadFactor = static_cast<float>(mCount) / static_cast<float>(mTable.size());
	return loadFactor > mThresholdLoadFactor;
}

void SpatialHashTable::ResizeTable()
{
}
