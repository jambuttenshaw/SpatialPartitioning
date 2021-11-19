#include "Quadtree.h"

#include "CollisionWorld.h"


Quadtree::Quadtree(const AABB& bounds)
	: SpatialPartition(bounds), mChildren({ nullptr, nullptr, nullptr, nullptr })
{
}

void Quadtree::Insert(ColliderID object, const AABB& bounds)
{
	// does this bounds intersect with this node?
	if (!mWorldBounds.Intersects(bounds))
		// the input does not intersect with this node
		// it has no place here
		return;

	// has this node split yet?
	if (mChildren[0] == nullptr)
	{
		// the node has not split
		// insert the object into this node
		mObjects.push_front(object);
		mObjectCount++;

		// should this object now split?
		if (mObjectCount > NODE_CAPACITY)
		{
			// split the node
			Split();
		}
	}
	else
	{
		// the node has split
		// insert input into children
		// note that one bounds may exist in multiple children,
		// this is the case when a bounds exists on the boundary between nodes
		for (auto& child : mChildren) child->Insert(object, bounds);
	}
}

void Quadtree::Delete(ColliderID object, const AABB& bounds)
{
	// needs rewrite: once split a node does not store any objects
	assert(false);
	if (mChildren[0] != nullptr)
	{
		int index = GetIndex(bounds);

		if (index == -1)
		{
			mObjects.remove(object);
		}
		else
		{
			mChildren[index]->Delete(object, bounds);
		}
	}
	else
	{
		mObjects.remove(object);
	}
}

void Quadtree::Split()
{
	float w = 0.5f * mWorldBounds.Size().x;
	float h = 0.5f * mWorldBounds.Size().y;
	float x = mWorldBounds.TopLeft().x;
	float y = mWorldBounds.TopLeft().y;

	mChildren[0] = new Quadtree({ x + w, y,     w, h }); // NE
	mChildren[1] = new Quadtree({ x + w, y + h, w, h }); // SE
	mChildren[2] = new Quadtree({ x,     y + h, w, h }); // SW
	mChildren[3] = new Quadtree({ x,     y,     w, h }); // NW

	// go through objects in this node and insert them into children
	for (auto& object : mObjects)
	{
		const auto& bounds = CollisionWorld::Instance()->Get(object);
		for (auto& child : mChildren) child->Insert(object, bounds);
	}
	// now that all the objects exist in the children, we can remove them from this node
	mObjects.clear();
	mObjectCount = 0;
}

int Quadtree::GetIndex(const AABB& bounds)
{
	// get rid of this function
	assert(false);


	// the object can completely fit in the top half of the space 
	// if the bottom of the bounds is higher than the midpoint of this node
	bool topHalf = bounds.TopLeft().y + bounds.Size().y < mWorldBounds.Centre().y;
	// object can completely fit in bottom half if top of bounds is below midpoint
	bool bottomHalf = bounds.TopLeft().y > mWorldBounds.Centre().y;

	// object can completely fit in left half if right is left of midpoint
	if (bounds.TopLeft().x + bounds.Size().x < mWorldBounds.Centre().x)
	{
		if (topHalf)
			return 3; // NW
		else if (bottomHalf)
			return 2; // SW
	}
	// object can completely fit in right half if left is right of midpoint
	else if (bounds.TopLeft().x > mWorldBounds.Centre().x)
	{
		if (topHalf)
			return 0; // NE
		else if (bottomHalf)
			return 1; // SE
	}

	// the bounds does not COMPLETELY fit in any of the quadrants
	return -1;
}

void Quadtree::Retrieve(std::vector<ColliderID>& out, const AABB& bounds)
{
	if (!mWorldBounds.Intersects(bounds))
	{
		// the bounds do not intersect with this node,
		// therefor it cannot collide with anything in this node or its children

		// do not modify the output vector
		return;
	}

	// the bounds DOES intersect with this node, so add all objects at this level into the output
	out.insert(out.end(), mObjects.begin(), mObjects.end());

	// check if there are children to this node
	if (mChildren[0] == nullptr)
	{
		// there are no children so proceed no further
		return;
	}

	for (auto& child : mChildren)
	{
		child->Retrieve(out, bounds);
	}
}
