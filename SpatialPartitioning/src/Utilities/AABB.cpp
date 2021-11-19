#include "AABB.h"

Vector2f AxisAlignedBoundingBox::Centre() const
{
    return mPosition - (0.5f * mSize);
}

bool AxisAlignedBoundingBox::Contains(const Vector2f& point) const
{
    return  (point.x > mPosition.x && point.x < mPosition.x + mSize.x) &&
            (point.y > mPosition.y && point.y < mPosition.y + mSize.y);
}

bool AxisAlignedBoundingBox::Contains(const AxisAlignedBoundingBox& aabb) const
{
    return Contains(aabb.TopLeft()) && Contains(aabb.TopLeft() + aabb.Size());
}

bool AxisAlignedBoundingBox::Intersects(const AxisAlignedBoundingBox& other) const
{
    if (mPosition.x + mSize.x < other.mPosition.x) return false;
    if (mPosition.x > other.mPosition.x + other.mSize.x) return false;
    if (mPosition.y + mSize.y < other.mPosition.y) return false;
    if (mPosition.y > other.mPosition.y + other.mSize.y) return false;

    return true;
}
