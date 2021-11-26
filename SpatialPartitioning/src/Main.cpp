#include <vector>

#include "Utilities/Random.h"
#include "Utilities/Instrumentor.h"
#include "CollisionWorld.h"
#include "SpatialHashTable.h"
#include "Quadtree.h"

#include <iostream>
#include <cassert>


AABB worldBounds{ {0, 0}, {100, 100} };


void OneIteration()
{
	// place 300 colliders into the world
	const size_t colliderCount = 1000;
	ColliderID ids[colliderCount];
	for (size_t i = 0; i < colliderCount; i++)
	{
		Vector2 newSize = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.Size() * 0.1f);
		//Vector2f newSize = { 2, 2 };
		Vector2f newPosition = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
		ids[i] = CollisionWorld::Instance()->AddAABB({ newPosition, newSize });
	}

	size_t collisionCount = 0;
	{
		PROFILE_SCOPE("SingleIteration");
		for (size_t i = 0; i < colliderCount; i++)
		{
			collisionCount += CollisionWorld::Instance()->GetCollisions(ids[i]).size();
		}
	}
	std::cout << "Number of collisions found: " << collisionCount << std::endl;

	CollisionWorld::Instance()->Clear();
}


int main()
{
	Random::Init();
	BEGIN_PROFILE_SESSION("Quadtree", "Quadtree.csv", false);

	// set up the collision world
	CollisionWorld::Instance()->SetWorldBounds(worldBounds);
	CollisionWorld::Instance()->SetSpatialPartitioner<Quadtree>();
	

	const size_t numIterations = 100;
	for (size_t i = 0; i < numIterations; i++)
	{
		OneIteration();
	}


	Random::Shutdown();
}
