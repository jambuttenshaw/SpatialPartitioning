#include <vector>

#include "Utilities/Random.h"
#include "Utilities/Instrumentor.h"

#include "CollisionWorld.h"
#include "SpatialHashTable.h"
#include "Quadtree.h"

#include "Tests/Tests.h"

#include <iostream>


AABB worldBounds{ {0, 0}, {100, 100} };

const size_t cases = 3;
const size_t colliderCounts[]
{
	10,
	100,
	500,
	1000,
	2500,
	5000
};


void RunAllTestsForAllCases()
{
	for (size_t j = 0; j < cases; j++)
	{
		std::cout << "Running case " << colliderCounts[j] << std::endl;
		RunAllTests(colliderCounts[j]);
	}
}


int main()
{
	Random::Init();
	CollisionWorld::Instance()->SetWorldBounds(worldBounds);

	std::cout << "Testing Brute Force Method" << std::endl;
	BEGIN_PROFILE_SESSION("BruteForce", "Results.csv", false);
	RunAllTestsForAllCases();
	END_PROFILE_SESSION();


	std::cout << "Testing Spatial Hash Table Method" << std::endl;
	BEGIN_PROFILE_SESSION("SpatialHashTable", "Results.csv", true);
	CollisionWorld::Instance()->SetSpatialPartitioner<SpatialHashTable>();
	RunAllTestsForAllCases();
	END_PROFILE_SESSION();

	
	std::cout << "Testing Quadtree Method" << std::endl;
	BEGIN_PROFILE_SESSION("Quadtree", "Results.csv", true);
	CollisionWorld::Instance()->SetSpatialPartitioner<Quadtree>();
	RunAllTestsForAllCases();
	END_PROFILE_SESSION();
	


	Random::Shutdown();
}
