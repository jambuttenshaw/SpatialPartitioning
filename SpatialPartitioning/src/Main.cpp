#include <vector>

#include "CollisionWorld.h"
#include "SpatialHashTable/SpatialHashTable.h"

int main()
{
	AABB worldBounds{ {0, 0}, {2, 2} };
	CollisionWorld* collisionWorld = CollisionWorld::Create<SpatialHashTable>(worldBounds);

	delete collisionWorld;
}
