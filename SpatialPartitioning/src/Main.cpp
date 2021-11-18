#include <vector>

#include "CollisionWorld.h"
#include "SpatialHashTable/SpatialHashTable.h"

#include <cassert>


int main()
{
	AABB worldBounds{ {0, 0}, {20, 20} };
	CollisionWorld* collisionWorld = CollisionWorld::Create<SpatialHashTable>(worldBounds);

	size_t ids[25];
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			ids[(5 * x) + y] = collisionWorld->CreateAABB({ (4.0f * x) - 8.0f, (4.0f * y) - 8.0f, 0.1f, 0.1f });
		}
	}

	for (int i = 0; i < 25; i++)
	{
		assert(collisionWorld->GetCollisions(ids[i]).size() == 0);
	}

	for (int i = 0; i < 25; i++)
	{
		collisionWorld->DeleteAABB(ids[i]);
	}

	delete collisionWorld;
}
