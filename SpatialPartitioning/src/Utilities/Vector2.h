#pragma once


class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;

	float sqr_magnitude();
	float magnitude();

	void normalise();
	Vector2 normalised();

	float dot(const Vector2& rhs);

	float x;
	float y;
};

// Vector2 operators

Vector2 operator+(Vector2 lhs, const Vector2& rhs);
Vector2 operator-(Vector2 lhs, const Vector2& rhs);
Vector2 operator*(Vector2 lhs, float rhs);
Vector2 operator*(float lhs, const Vector2& rhs);
Vector2 operator/(Vector2 lhs, float rhs);

void operator+=(Vector2& lhs, const Vector2& rhs);
void operator-=(Vector2& lhs, const Vector2& rhs);
void operator*=(Vector2& lhs, float rhs);
void operator/=(Vector2& lhs, float rhs);

bool operator==(Vector2 lhs, const Vector2& rhs);
