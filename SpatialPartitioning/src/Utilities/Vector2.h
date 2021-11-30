#pragma once

#include <cmath>
#include <algorithm>

template<typename T>
class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(T _x, T _y) : x(_x), y(_y) {}

	inline T sqr_magnitude()
	{
		return x * x + y * y;
	}
	inline float magnitude()
	{
		return sqrtf(static_cast<float>(sqr_magnitude()));
	}

	inline void normalise()
	{
		float mag = magnitude();
		x /= mag;
		y /= mag;
	}
	inline Vector2<T> normalised()
	{
		float mag = magnitude();
		return Vector2<T>{ x / mag, y / mag };
	}

	inline T dot(const Vector2<T>& rhs)
	{
		return x * rhs.x + y * rhs.y;
	}

	T x = T();
	T y = T();


	inline static Vector2<T> Min(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
	}

	inline static Vector2<T> Max(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
	}


	static const Vector2<float> Zero;
};

// Vector2 operators

template<typename T>
Vector2<T> operator+(Vector2<T> lhs, const Vector2<T>& rhs)
{
	return Vector2<T>{ lhs.x + rhs.x, lhs.y + rhs.y };
}
template<typename T>
Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs)
{
	return Vector2<T>{ lhs.x - rhs.x, lhs.y - rhs.y };
}
template<typename T, typename U>
Vector2<T> operator*(Vector2<T> lhs, U rhs)
{
	return Vector2<T>{ lhs.x * rhs, lhs.y * rhs };
}
template<typename T, typename U>
Vector2<T> operator*(U lhs, const Vector2<T>& rhs)
{
	return Vector2<T>{ rhs.x * lhs, rhs.y * lhs };
}
template<typename T, typename U>
Vector2<T> operator/(Vector2<T> lhs, U rhs)
{
	return Vector2<T>{ lhs.x / rhs, lhs.y / rhs };
}

template<typename T>
void operator+=(Vector2<T>& lhs, const Vector2<T>& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
}
template<typename T>
void operator-=(Vector2<T>& lhs, const Vector2<T>& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
}
template<typename T, typename U>
void operator*=(Vector2<T>& lhs, U rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
}
template<typename T, typename U>
void operator/=(Vector2<T>& lhs, U rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
}


template<typename T>
bool operator==(Vector2<T> lhs, const Vector2<T>& rhs)
{
	static const float epsilon = 0.000001f;
	return ((fabsf(static_cast<float>(lhs.x - rhs.x)) < epsilon) &&
			(fabsf(static_cast<float>(lhs.y - rhs.y)) < epsilon));
}
template<typename T>
bool operator!=(Vector2<T> lhs, const Vector2<T>& rhs)
{
	return !(lhs == rhs);
}


using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
