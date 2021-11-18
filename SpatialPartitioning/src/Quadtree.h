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

	virtual void Insert(ColliderID object) override;
	virtual void Delete(ColliderID object) override;

	virtual std::vector<ColliderID> Retrieve(const AABB& bounds) override;

private:

	inline void SetLevel(int level) { mLevel = level; }

private:
	
	// the level this partition is located on
	int mLevel = 0;
	// objects inside this partition
	std::forward_list<ColliderID> mObjects;
	
	// the subtrees of this node
	std::array<Quadtree*, 4> mChildren;
};