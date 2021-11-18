#pragma once

#include "Vector2.h"

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox(float x, float y, float width, float height) : mCentre(x, y), mSize(width, height) {}
	AxisAlignedBoundingBox(const Vector2f& centre, const Vector2f& size) : mCentre(centre), mSize(size) {}

	inline const Vector2f& Centre() const { return mCentre; }
	inline const Vector2f& Size() const { return mSize; }
	Vector2f TopLeft() const;

	inline void Resize(const Vector2f& size) { mSize = size; }
	
	inline void SetPosition(const Vector2f& pos) { mCentre = pos; }
	inline void Translate(const Vector2f& translation) { mCentre += translation; }

	// Intersection checks
	bool Contains(const Vector2f& point) const;
	bool Intersects(const AxisAlignedBoundingBox& other) const;

private:
	Vector2f mCentre, mSize;
};

typedef AxisAlignedBoundingBox AABB;