#pragma once

#include "Utilities/Constants.h"


// run every test
void RunAllTests(size_t colliderCount);


// test declarations
void RandomSizeAndPositionTest(size_t colliderCount, ColliderID* ids);
void LargeObjectsTest(size_t colliderCount, ColliderID* ids);
void SmallObjectsTest(size_t colliderCount, ColliderID* ids);
void EvenlySpreadObjectsTest(size_t colliderCount, ColliderID* ids);
void DenselyClumpedObjectsTest(size_t colliderCount, ColliderID* ids);
void WorldSizesTest(size_t colliderCount, ColliderID* ids);
void InsertionOverheadTest(size_t colliderCount, ColliderID* ids);
void DeletionOverheadTest(size_t colliderCount, ColliderID* ids);

// quadtree specific tests
void NodeLimitTest(size_t colliderCount, ColliderID* ids);
void NodeCapacityTest(size_t colliderCount, ColliderID* ids);

// hash table specific tests
void CellSizeTest(size_t colliderCount, ColliderID* ids);