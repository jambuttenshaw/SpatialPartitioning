#include "Quadtree.h"

Quadtree::Quadtree(const AABB& bounds)
	: SpatialPartition(bounds)
{
}

void Quadtree::Insert(ColliderID object)
{
}

void Quadtree::Delete(ColliderID object)
{
}

std::vector<ColliderID> Quadtree::Retrieve(const AABB& bounds)
{
	return std::vector<ColliderID>();
}
