#include <vector>

#include "Utilities/Random.h"
#include "Utilities/Instrumentor.h"
#include "CollisionWorld.h"
#include "SpatialHashTable.h"
#include "Quadtree.h"

#include <iostream>
#include <cassert>


int main()
{
	Random::Init();

	// set up the collision world
	AABB worldBounds{ {0, 0}, {100, 100} };
	CollisionWorld::Instance()->SetWorldBounds(worldBounds);
	CollisionWorld::Instance()->SetSpatialPartitioner<SpatialHashTable>();

	// place 300 colliders into the world
	const size_t colliderCount = 300;
	ColliderID ids[colliderCount];
	for (size_t i = 0; i < colliderCount; i++)
	{
		Vector2 newSize = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.Size() * 0.1f);
		//Vector2f newSize = { 2, 2 };
		Vector2f newPosition = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
		ids[i] = CollisionWorld::Instance()->AddAABB({ newPosition, newSize });
	}

	size_t collisionCount = 0;
	for (size_t i = 0; i < colliderCount; i++)
	{
		//PROFILE_SCOPE("GET COLLISIONS");
		collisionCount += CollisionWorld::Instance()->GetCollisions(ids[i]).size();
	}

	// collision pairs will be counted twice
	// collision count should be an even number

	std::cout << "Number of collisions found: " << collisionCount << std::endl;

	CollisionWorld::Instance()->Clear();

	Random::Shutdown();
}
