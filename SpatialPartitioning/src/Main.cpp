#include <vector>

#include "Utilities/Random.h"
#include "Utilities/Instrumentor.h"

#include "CollisionWorld.h"
#include "SpatialHashTable.h"
#include "Quadtree.h"

#include "Tests/Tests.h"


AABB worldBounds{ {0, 0}, {100, 100} };

const size_t colliderCounts[4]
{
	10,
	100,
	500,
	1000
};


int main()
{
	Random::Init();
	BEGIN_PROFILE_SESSION("Quadtree", "Quadtree.csv", false);

	// set up the collision world
	CollisionWorld::Instance()->SetWorldBounds(worldBounds);
	CollisionWorld::Instance()->SetSpatialPartitioner<Quadtree>();
	
	for (size_t j = 0; j < 4; j++) 
	{
		RunAllTests(colliderCounts[j]);
	}


	Random::Shutdown();
}
