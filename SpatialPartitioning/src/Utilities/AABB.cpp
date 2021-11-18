#include "AABB.h"

Vector2f AxisAlignedBoundingBox::TopLeft() const
{
    return mCentre - (0.5f * mSize);
}

bool AxisAlignedBoundingBox::Contains(const Vector2f& point) const
{
    Vector2f topleft = TopLeft();
    return  (point.x > topleft.x && point.x < topleft.x + mSize.x) &&
            (point.y > topleft.y && point.y < topleft.y + mSize.y);
}

bool AxisAlignedBoundingBox::Intersects(const AxisAlignedBoundingBox& other) const
{
    Vector2f topleft = TopLeft();
 
    Vector2f otherTopleft = other.TopLeft();
    Vector2f otherSize = other.Size();

    return (otherTopleft.x < topleft.x + mSize.x || topleft.y < otherTopleft.x + otherSize.x) &&
        (otherTopleft.y < topleft.y + mSize.y || topleft.y < otherTopleft.y + otherSize.y);
}
