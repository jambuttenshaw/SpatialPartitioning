#include "Tests.h"

#include "Utilities/Random.h"
#include "Utilities/Instrumentor.h"

#include "CollisionWorld.h"


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
	WorldSizesTest(colliderCount, ids);
	InsertionOverheadTest(colliderCount, ids);
	DeletionOverheadTest(colliderCount, ids);

	delete[] ids;
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

		CollisionWorld::Instance()->Resize(id, newSize);
		CollisionWorld::Instance()->SetPosition(id, newPos);
	}


	size_t collisionCount = 0;
	for (size_t iteration = 0; iteration < gTestIterations; iteration++)
	{
		for (size_t i = 0; i < colliderCount; i++)
		{
			PROFILE_SCOPE_AVERAGE("RandomSizeAndPosition" + std::to_string(colliderCount));
			collisionCount += CollisionWorld::Instance()->GetCollisions(ids[i]).size();
		}
	}
}

void LargeObjectsTest(size_t colliderCount, ColliderID* ids)
{
}

void SmallObjectsTest(size_t colliderCount, ColliderID* ids)
{
}

void EvenlySpreadObjectsTest(size_t colliderCount, ColliderID* ids)
{
}

void DenselyClumpedObjectsTest(size_t colliderCount, ColliderID* ids)
{
}

void WorldSizesTest(size_t colliderCount, ColliderID* ids)
{
}

void InsertionOverheadTest(size_t colliderCount, ColliderID* ids)
{
}

void DeletionOverheadTest(size_t colliderCount, ColliderID* ids)
{
}



void NodeLimitTest(size_t colliderCount, ColliderID* ids)
{
}

void NodeCapacityTest(size_t colliderCount, ColliderID* ids)
{
}



void CellSizeTest(size_t colliderCount, ColliderID* ids)
{
}
