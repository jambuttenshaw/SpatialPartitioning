#include <vector>

#include "CollisionWorld.h"
#include "SpatialHashTable.h"

#include <cassert>


int main()
{
	AABB worldBounds{ {0, 0}, {20, 20} };
	CollisionWorld* collisionWorld = CollisionWorld::Create<SpatialHashTable>(worldBounds);

	delete collisionWorld;
}
