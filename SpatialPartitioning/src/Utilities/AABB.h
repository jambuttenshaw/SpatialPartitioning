#pragma once

#include "Vector2.h"

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox(const Vector2& centre, const Vector2& size) : mCentre(centre), mSize(size) {}

	inline const Vector2& Centre() const { return mCentre; }
	inline const Vector2& Size() const { return mSize; }
	Vector2 TopLeft() const;

	inline void Resize(const Vector2& size) { mSize = size; }
	
	inline void SetPosition(const Vector2& pos) { mCentre = pos; }
	inline void Translate(const Vector2& translation) { mCentre += translation; }

	// Intersection checks
	bool Contains(const Vector2& point);
	bool Intersects(const AxisAlignedBoundingBox& other);

private:
	Vector2 mCentre, mSize;
};

typedef AxisAlignedBoundingBox AABB;