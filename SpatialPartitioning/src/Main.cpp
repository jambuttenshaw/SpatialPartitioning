#include <vector>

#include "CollisionWorld.h"
#include "SpatialHashTable.h"

#include <cassert>


int main()
{
	// set up the collision world
	AABB worldBounds{ {0, 0}, {20, 20} };
	CollisionWorld::Instance()->SetWorldBounds(worldBounds);
	CollisionWorld::Instance()->SetSpatialPartitioner<SpatialHashTable>();
}
