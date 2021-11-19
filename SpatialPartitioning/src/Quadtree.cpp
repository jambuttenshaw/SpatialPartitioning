#include "Quadtree.h"

#include "CollisionWorld.h"


Quadtree::Quadtree(const AABB& bounds)
	: SpatialPartition(bounds), mChildren({ nullptr, nullptr, nullptr, nullptr })
{
}

Quadtree::~Quadtree()
{
	for (auto& child : mChildren) delete child;
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
	// if the bounds does not intersect with this node do nothing
	if (!mWorldBounds.Intersects(bounds))
		return;

	// if this node has children then seach them to delete the object
	// otherwise delete from this object
	if (mChildren[0] == nullptr)
	{
		mObjects.remove(object);
		mObjectCount--;
	}
	else
	{
		for (auto& child : mChildren) child->Delete(object, bounds);
	}
}

void Quadtree::Clear()
{
	mObjects.clear();
	mObjectCount = 0;
	if (mChildren[0] != nullptr)
	{
		// destroy children
		for (auto& child : mChildren)
		{
			delete child;
			child = nullptr;
		}
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


void Quadtree::Retrieve(std::vector<ColliderID>& out, const AABB& bounds)
{
	if (!mWorldBounds.Intersects(bounds))
	{
		// the bounds do not intersect with this node,
		// therefore it cannot collide with anything in this node or its children

		// do not modify the output vector
		return;
	}

	// check if there are children to this node
	if (mChildren[0] == nullptr)
	{
		// there are no children, this is a leaf of the quadtree
		// add any objects in this node
		out.insert(out.end(), mObjects.begin(), mObjects.end());
	}
	else
	{
		for (auto& child : mChildren)
		{
			child->Retrieve(out, bounds);
		}
	}
}
