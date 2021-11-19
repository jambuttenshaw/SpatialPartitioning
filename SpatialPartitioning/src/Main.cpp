#include <vector>

#include "Utilities/Random.h"
#include "CollisionWorld.h"
#include "SpatialHashTable.h"
#include "Quadtree.h"

#include <cassert>


int main()
{
	Random::Init();

	// set up the collision world
	AABB worldBounds{ {0, 0}, {100, 100} };
	CollisionWorld::Instance()->SetWorldBounds(worldBounds);
	CollisionWorld::Instance()->SetSpatialPartitioner<Quadtree>();

	// place 300 colliders into the world
	const size_t colliderCount = 300;
	for (size_t i = 0; i < colliderCount; i++)
	{
		Vector2 newSize = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.Size() * 0.1f);
		Vector2 newPosition = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
		CollisionWorld::Instance()->AddAABB({ newPosition, newSize });
	}

	CollisionWorld::Instance()->Clear();

	Random::Shutdown();
}
