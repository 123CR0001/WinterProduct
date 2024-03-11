#pragma once
#include<cmath>

class Vector2 {
public:

	float x, y;

	Vector2()
		:x(0.f)
		,y(0.f)
	{
	}

	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vector2(const Vector2& v) {
		*this = v;
	}

	friend Vector2 operator+(const Vector2& left,const Vector2& right)
	{
		return Vector2(left.x + right.x, left.y + right.y);
	}
	friend Vector2 operator-(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x - right.x, left.y - right.y);
	}

	friend Vector2 operator* (const Vector2& left, const float scale) {
		return Vector2(left.x * scale, left.y * scale);
	}

	friend Vector2 operator/ (const Vector2& left, const float scale) {
		return Vector2(left.x / scale, left.y / scale);
	}

	const Vector2& operator+=(const Vector2& right) {
		x += right.x;
		y += right.y;
		return *this;
	}

	const Vector2& operator-=(const Vector2& right) {
		x -= right.x;
		y -= right.y;
		return *this;
	}

	const Vector2& operator*=(const float scale) {
		x *= scale;
		y *= scale;
		return *this;
	}

	const Vector2& operator/=(const float scale) {
		x /= scale;
		y /= scale;
		return *this;
	}

	static Vector2 RadTo(const float rad) {
		return Vector2(cosf(rad), sinf(rad));
	}

	static const float Dot(const Vector2& left,const Vector2& right){
		return left.x * right.x + left.y + right.y;
	}

	const float LengthSquare() { return Dot(*this, *this); }

	static const float LengthSquare(const Vector2& left, const Vector2& right) {
		return Vector2(left - right).LengthSquare();
	}

	const float Length() { return sqrtf(LengthSquare()); }

	static const float Length(const Vector2& left, const Vector2& right) {
		return Vector2(left - right).Length();
	}

	const Vector2& Normalize() { return *this / Length(); }

	void Normalized() { *this = *this / Length(); }

	static const Vector2& Lerp(const Vector2& start, const Vector2& end, const float rate) {
		return start + (end - start) * rate;
	}
};
