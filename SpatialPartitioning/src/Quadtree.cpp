#include "Quadtree.h"

#include "CollisionWorld.h"


Quadtree::Quadtree(const AABB& bounds)
	: SpatialPartition(bounds), mChildren({ nullptr, nullptr, nullptr, nullptr })
{
}

void Quadtree::Insert(ColliderID object, const AABB& bounds)
{
	if (mChildren[0] != nullptr)
	{
		// this node has split
		int index = GetIndex(bounds);
		if (index != -1)
			mChildren[index]->Insert(object, bounds);
	}

	mObjects.push_front(object);
	mObjectCount++;

	if (mObjectCount > NODE_CAPACITY)
	{
		if (mChildren[0] == nullptr)
			Split();

		auto it = mObjects.begin();
		while (it != mObjects.end())
		{
			const AABB& childBouns = CollisionWorld::Instance()->Get(*it);
			int index = GetIndex(childBouns);

			if (index == -1)
			{
				// dont do anything to this one, it doesn't get placed into a sub-node
				it++;
			}
			else
			{
				mChildren[index]->Insert(*it, CollisionWorld::Instance()->Get(*it));
				// remove object from this nodes objects, as it will belong to its children
				it = mObjects.erase(it);
				mObjectCount--;
			}
		}
	}
}

void Quadtree::Delete(ColliderID object, const AABB& bounds)
{
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
}

int Quadtree::GetIndex(const AABB& bounds)
{
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
