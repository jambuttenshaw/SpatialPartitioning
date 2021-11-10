#include "Vector2.h"

#include <cmath>


constexpr static float epsilon = 0.00001f;

float Vector2::sqr_magnitude()
{
    return x * x + y * y;
}

float Vector2::magnitude()
{
    return sqrtf(sqr_magnitude());
}

void Vector2::normalise()
{
    float mag = magnitude();
    x /= mag;
    y /= mag;
}

Vector2 Vector2::normalised()
{
    float mag = magnitude();
    return Vector2{ x / mag, y / mag };
}

float Vector2::dot(const Vector2& rhs)
{
    return x * rhs.x + y * rhs.y;
}

Vector2 operator+(Vector2 lhs, const Vector2& rhs)
{
    return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

Vector2 operator-(Vector2 lhs, const Vector2& rhs)
{
    return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

Vector2 operator*(Vector2 lhs, float rhs)
{
    return Vector2{ lhs.x * rhs, lhs.y * rhs };
}

Vector2 operator*(float lhs, const Vector2& rhs)
{
    return Vector2{ rhs.x * lhs, rhs.y * lhs };
}

Vector2 operator/(Vector2 lhs, float rhs)
{
    return Vector2{ lhs.x / rhs, lhs.y / rhs };
}

void operator+=(Vector2& lhs, const Vector2& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
}

void operator-=(Vector2& lhs, const Vector2& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
}

void operator*=(Vector2& lhs, float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
}

void operator/=(Vector2& lhs, float rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
}

bool operator==(Vector2 lhs, const Vector2& rhs)
{
    return ((fabsf(lhs.x - rhs.x) < epsilon) &&
            (fabsf(lhs.y - rhs.y) < epsilon));
}
