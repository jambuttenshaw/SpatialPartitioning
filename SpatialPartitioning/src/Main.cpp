#include <vector>

#include "CollisionWorld.h"
#include "SpatialHashTable.h"
#include "Quadtree.h"

#include <cassert>


int main()
{
	// set up the collision world
	AABB worldBounds{ {0, 0}, {20, 20} };
	CollisionWorld::Instance()->SetWorldBounds(worldBounds);
	CollisionWorld::Instance()->SetSpatialPartitioner<Quadtree>();

	ColliderID ids[25];
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
			ids[(x * 5) + y] = CollisionWorld::Instance()->AddAABB({ 4.0f * x + 2.0f, 4.0f * y + 2.0f, 0.1f, 0.1f });
	}

	for (int i = 0; i < 25; i++)
		CollisionWorld::Instance()->DeleteAABB(ids[i]);
}
