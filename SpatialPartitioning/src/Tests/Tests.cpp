#include "Tests.h"

#include "Utilities/Random.h"
#include "Utilities/Instrumentor.h"

#include "CollisionWorld.h"
#include "SpatialHashTable.h"
#include "Quadtree.h"

#include <array>


static const size_t gTestIterations = 300;


void RunAllTests(size_t colliderCount)
{
	CollisionWorld::Instance()->Clear();

	ColliderID* ids = new ColliderID[colliderCount];
	for (size_t i = 0; i < colliderCount; i++)
	{
		ids[i] = CollisionWorld::Instance()->AddAABB({0.1f, 0.1f, 0.1f, 0.1f});
	}

	RandomSizeAndPositionTest(colliderCount, ids);
	LargeObjectsTest(colliderCount, ids);
	SmallObjectsTest(colliderCount, ids);
	EvenlySpreadObjectsTest(colliderCount, ids);
	DenselyClumpedObjectsTest(colliderCount, ids);

	// the colliders present are no longer needed
	CollisionWorld::Instance()->Clear();
	delete[] ids;

	WorldSizesTest(colliderCount);
	InsertionAndDeletionOverheadTest(colliderCount);
}


/*
	TEST IMPLEMENTATIONS
*/

void RandomSizeAndPositionTest(size_t colliderCount, ColliderID* ids)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	for (size_t i = 0; i < colliderCount; i++)
	{
		ColliderID id = ids[i];
		
		Vector2f newSize = Random::RandomVector({ 0.2f, 0.2f }, { 5.0f, 5.0f });
		Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);

		CollisionWorld::Instance()->ResetObject(id, { newPos, newSize });
	}


	PerformCollisionTest("RandomSizeAndPosition", colliderCount, ids);
}

void LargeObjectsTest(size_t colliderCount, ColliderID* ids)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	for (size_t i = 0; i < colliderCount; i++)
	{
		ColliderID id = ids[i];

		Vector2f newSize = Random::RandomVector({ 10.0f, 10.0f }, { 30.0f, 30.0f });
		Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);

		CollisionWorld::Instance()->ResetObject(id, { newPos, newSize });
	}


	PerformCollisionTest("LargeObjects", colliderCount, ids);
}

void SmallObjectsTest(size_t colliderCount, ColliderID* ids)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	for (size_t i = 0; i < colliderCount; i++)
	{
		ColliderID id = ids[i];

		Vector2f newSize = Random::RandomVector({ 0.1f, 0.1f }, { 1.5f, 1.5f });
		Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);

		CollisionWorld::Instance()->ResetObject(id, { newPos, newSize });
	}


	PerformCollisionTest("SmallObjects", colliderCount, ids);
}

void EvenlySpreadObjectsTest(size_t colliderCount, ColliderID* ids)
{
	// find out how many objects to spread along each axis
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	float b = sqrtf(static_cast<float>(colliderCount * worldBounds.Size().y) / static_cast<float>(worldBounds.Size().x));
	float a = colliderCount / b;

	float countX = ceilf(a);
	float countY = ceilf(b);
	// countX * countY will always be greater than colliderCount

	Vector2f spacing{ worldBounds.Size().x / (countX + 1), 
					  worldBounds.Size().y / (countY + 1) };
	Vector2f colliderSize = Vector2f::Min({ 2.0f, 2.0f }, spacing);
	
	for (size_t i = 0; i < colliderCount; i++)
	{
		ColliderID id = ids[i];

		float x = fmodf(static_cast<float>(i), countX) * spacing.x;
		float y = floorf(i / countY) * spacing.y;

		CollisionWorld::Instance()->ResetObject(id, { { x, y }, colliderSize });
	}
	

	PerformCollisionTest("EvenlySpreadObjects", colliderCount, ids);
}

void DenselyClumpedObjectsTest(size_t colliderCount, ColliderID* ids)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	Vector2f range{ 5.0f, 5.0f };
	Vector2f centre = Random::RandomVector(1.5f * range, worldBounds.TopLeft() + worldBounds.Size() - 1.5f * range);

	for (size_t i = 0; i < colliderCount; i++)
	{
		ColliderID id = ids[i];
		
		Vector2f pos = Random::RandomVector(centre - range, centre + range);
		CollisionWorld::Instance()->ResetObject(id, { pos, { 2.0f, 2.0f } });
	}

	PerformCollisionTest("DenselyPackedObjects", colliderCount, ids);
}

void WorldSizesTest(size_t colliderCount)
{
	ColliderID* ids = new ColliderID[colliderCount];


	std::array<AABB, 4> worldBoundsCases = {
		AABB{0.0f, 0.0f, 10.0f, 10.0f},
		AABB{0.0f, 0.0f, 50.0f, 50.0f},
		AABB{0.0f, 0.0f, 100.0f, 100.0f},
		AABB{0.0f, 0.0f, 250.0f, 250.0f}
	};

	// for each cell size:
	//		insert a number of colliders into the spatial partition
	//		run tests
	//		clear
	for (const AABB& worldBounds : worldBoundsCases)
	{
		CollisionWorld::Instance()->Clear();
		CollisionWorld::Instance()->SetWorldBounds(worldBounds);


		// insert into spatial hash table
		for (size_t i = 0; i < colliderCount; i++)
		{
			Vector2f newSize = Random::RandomVector({ 0.2f, 0.2f }, { 5.0f, 5.0f });
			Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
			ids[i] = CollisionWorld::Instance()->AddAABB({ newPos, newSize });
		}

		std::stringstream testName;
		testName << "WorldSize" << static_cast<int>(worldBounds.Size().x);
		testName << "x" << static_cast<int>(worldBounds.Size().y);
		PerformCollisionTest(testName.str(), colliderCount, ids);
	}

	delete[] ids;
}

void InsertionAndDeletionOverheadTest(size_t colliderCount)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	ColliderID* ids = new ColliderID[colliderCount];

	
	for (size_t iteration = 0; iteration < gTestIterations; iteration++)
	{
		for (size_t i = 0; i < colliderCount; i++)
		{
			Vector2f newSize = Random::RandomVector({ 0.2f, 0.2f }, { 5.0f, 5.0f });
			Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
			{
				PROFILE_SCOPE_AVERAGE("InsertionOverhead" + std::to_string(colliderCount));
				ids[i] = CollisionWorld::Instance()->AddAABB({ newPos, newSize });
			}
		}

		for (size_t i = 0; i < colliderCount; i++)
		{
			PROFILE_SCOPE_AVERAGE("DeletionOverhead" + std::to_string(colliderCount));
			CollisionWorld::Instance()->DeleteAABB(ids[i]);
		}
	}


	delete[] ids;
}



void NodeLimitTest(size_t colliderCount)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	ColliderID* ids = new ColliderID[colliderCount];


	std::array<size_t, 4> nodeLimitCases = {
		3,
		5,
		7,
		10
	};

	// for each cell size:
	//		insert a number of colliders into the spatial partition
	//		run tests
	//		clear
	for (size_t nodeLimit : nodeLimitCases)
	{
		CollisionWorld::Instance()->Clear();

		Quadtree* quadtree = static_cast<Quadtree*>(CollisionWorld::Instance()->GetSpatialPartition());
		quadtree->SetNodeLimit(nodeLimit);

		// insert into spatial hash table
		for (size_t i = 0; i < colliderCount; i++)
		{
			Vector2f newSize = Random::RandomVector({ 0.2f, 0.2f }, { 5.0f, 5.0f });
			Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
			ids[i] = CollisionWorld::Instance()->AddAABB({ newPos, newSize });
		}

		PerformCollisionTest("NodeLimit" + std::to_string(nodeLimit), colliderCount, ids);
	}

	delete[] ids;
}

void NodeCapacityTest(size_t colliderCount)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	ColliderID* ids = new ColliderID[colliderCount];


	std::array<size_t, 4> nodeCapacityCases = {
		5,
		10,
		15,
		20
	};

	// for each cell size:
	//		insert a number of colliders into the spatial partition
	//		run tests
	//		clear
	for (size_t nodeCapacity : nodeCapacityCases)
	{
		CollisionWorld::Instance()->Clear();

		Quadtree* quadtree = static_cast<Quadtree*>(CollisionWorld::Instance()->GetSpatialPartition());
		quadtree->SetNodeCapacity(nodeCapacity);

		// insert into spatial hash table
		for (size_t i = 0; i < colliderCount; i++)
		{
			Vector2f newSize = Random::RandomVector({ 0.2f, 0.2f }, { 5.0f, 5.0f });
			Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
			ids[i] = CollisionWorld::Instance()->AddAABB({ newPos, newSize });
		}

		PerformCollisionTest("NodeCapacity" + std::to_string(nodeCapacity), colliderCount, ids);
	}

	delete[] ids;
}



void CellSizeTest(size_t colliderCount)
{
	const AABB& worldBounds = CollisionWorld::Instance()->GetWorldBounds();

	ColliderID* ids = new ColliderID[colliderCount];


	std::array<float, 4> cellSizeCases = {
		2.0f,
		4.0f,
		8.0f,
		16.0f
	};

	// for each cell size:
	//		insert a number of colliders into the spatial partition
	//		run tests
	//		clear
	for (float cellSize : cellSizeCases)
	{
		CollisionWorld::Instance()->Clear();
		
		SpatialHashTable* sHT = static_cast<SpatialHashTable*>(CollisionWorld::Instance()->GetSpatialPartition());
		sHT->SetCellSize(cellSize);

		// insert into spatial hash table
		for (size_t i = 0; i < colliderCount; i++)
		{
			Vector2f newSize = Random::RandomVector({ 0.2f, 0.2f }, { 5.0f, 5.0f });
			Vector2f newPos = Random::RandomVector({ 0.1f, 0.1f }, worldBounds.TopLeft() + worldBounds.Size() - newSize);
			ids[i] = CollisionWorld::Instance()->AddAABB({ newPos, newSize });
		}

		PerformCollisionTest("CellSize" + std::to_string(static_cast<int>(cellSize)), colliderCount, ids);
	}

	delete[] ids;
}


void PerformCollisionTest(const std::string& testName, size_t colliderCount, ColliderID* ids)
{
	std::cout << "Running test: " << testName << std::endl;

	size_t collisionCount = 0;
	for (size_t iteration = 0; iteration < gTestIterations; iteration++)
	{
		for (size_t i = 0; i < colliderCount; i++)
		{
			PROFILE_SCOPE_AVERAGE(testName + "_" + std::to_string(colliderCount));
			collisionCount += CollisionWorld::Instance()->GetCollisions(ids[i]).size();
		}
	}
}
