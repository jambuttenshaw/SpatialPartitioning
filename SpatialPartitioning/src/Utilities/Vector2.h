#pragma once


class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float _x, float _y) : x(_x), y(_y) {}

	float sqr_magnitude()
	{
		return x * x + y * y;
	}
	float magnitude()
	{
		return sqrtf(static_cast<float>(sqr_magnitude()));
	}

	void normalise()
	{
		float mag = magnitude();
		x /= mag;
		y /= mag;
	}
	Vector2 normalised()
	{
		float mag = magnitude();
		return Vector2{ x / mag, y / mag };
	}

	float dot(const Vector2& rhs)
	{
		return x * rhs.x + y * rhs.y;
	}

	float x = float();
	float y = float();
};

// Vector2 operators


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
	static const float epsilon = 0.000001f;
	return ((fabsf(static_cast<float>(lhs.x - rhs.x)) < epsilon) &&
			(fabsf(static_cast<float>(lhs.y - rhs.y)) < epsilon));
}
