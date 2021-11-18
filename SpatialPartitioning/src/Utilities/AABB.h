#pragma once

#include "Vector2.h"

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox() = default;
	AxisAlignedBoundingBox(float x, float y, float width, float height) : mPosition(x, y), mSize(width, height) {}
	AxisAlignedBoundingBox(const Vector2f& topleft, const Vector2f& size) : mPosition(topleft), mSize(size) {}

	inline const Vector2f& TopLeft() const { return mPosition; }
	Vector2f Centre() const;
	inline const Vector2f& Size() const { return mSize; }

	inline void Resize(const Vector2f& size) { mSize = size; }
	
	inline void SetPosition(const Vector2f& pos) { mPosition = pos; }
	inline void Translate(const Vector2f& translation) { mPosition += translation; }

	// Intersection checks
	bool Contains(const Vector2f& point) const;
	bool Intersects(const AxisAlignedBoundingBox& other) const;

private:
	Vector2f mPosition, mSize;
};

typedef AxisAlignedBoundingBox AABB;