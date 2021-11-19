#pragma once

#include "SpatialPartition.h"
#include "Utilities/AABB.h"

#include <array>
#include <forward_list>

class Quadtree : public SpatialPartition
{
public:
	// quadtree inherits from spatial partition, and spatial partition provides a "world bounds" property
	// in the context of a node in the quadtree, the world bounds is actually the bounds that this partition of the quadtree controls
	// and not the size of the entire world
	Quadtree(const AABB& bounds);
	virtual ~Quadtree() = default;

	virtual void Insert(ColliderID object, const AABB& bounds) override;
	virtual void Delete(ColliderID object, const AABB& bounds) override;

	virtual void Retrieve(std::vector<ColliderID>& out, const AABB& bounds) override;

private:

	// split this node into 4 subnodes
	void Split();
	// get the quadrant a particular bounds would lie in
	// note: a signed type is used as the return value of -1 means that the bounds does not completely lie inside a quadrant
	int GetIndex(const AABB& bounds);

private:
	
	// objects inside this partition
	std::forward_list<ColliderID> mObjects;
	size_t mObjectCount = 0;
	
	// the subtrees of this node
	std::array<Quadtree*, 4> mChildren;

private:
	static inline constexpr size_t NODE_CAPACITY = 5;
};