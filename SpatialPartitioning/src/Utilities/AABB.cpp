#include "AABB.h"

Vector2 AxisAlignedBoundingBox::TopLeft() const
{
    return mCentre - (0.5f * mSize);
}

bool AxisAlignedBoundingBox::Contains(const Vector2& point)
{
    Vector2 topleft = TopLeft();
    return  (point.x > topleft.x && point.x < topleft.x + mSize.x) &&
            (point.y > topleft.y && point.y < topleft.y + mSize.y);
}

bool AxisAlignedBoundingBox::Intersects(const AxisAlignedBoundingBox& other)
{
    Vector2 topleft = TopLeft();
 
    Vector2 otherTopleft = other.TopLeft();
    Vector2 otherSize = other.Size();

    return (otherTopleft.x < topleft.x + mSize.x || topleft.y < otherTopleft.x + otherSize.x) &&
        (otherTopleft.y < topleft.y + mSize.y || topleft.y < otherTopleft.y + otherSize.y);
}
