#pragma once
#include<cmath>

class Vector3 {
public:
	Vector3() { x = 0.0f; y = 0.0f; z = 0.0f; }
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(const Vector3& right) {
		*this = right;
	}
	virtual ~Vector3() {}

	//DxLibのVECTOR構造体みたいに使用したいのでpublic
	float x, y, z;

	//値の代入
	const Vector3& operator = (const Vector3& right) {
		x = right.x;
		y = right.y;
		z = right.z;
		return *this;
	}

	//ベクトルの加算代入
	const Vector3& operator += (const Vector3& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	//ベクトルの減算代入
	const Vector3& operator -= (const Vector3& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	//ベクトルの乗算代入
	const Vector3& operator *= (const float right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	//ベクトルの除算代入
	const Vector3& operator /= (const float right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	//ベクトルの加算
	friend Vector3 operator+(const Vector3& left, const Vector3& right) {
		return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
	}

	//ベクトルの減算
	friend Vector3 operator-(const Vector3& left, const Vector3& right) {
		return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	//ベクトルの乗算
	friend Vector3 operator * (const Vector3& left, float right) {
		return Vector3(left.x * right, left.y * right, left.z * right);
	}

	//ベクトルの除算代入
	friend Vector3 operator / (const Vector3& left, float right) {
		return Vector3(left.x / right, left.y / right, left.z / right);
	}

	//二つの座標が等しいか
	bool operator == (const Vector3& right);

	//二つの座標が等しくないか
	bool operator != (const Vector3& right);

	//どちらのベクトルの方が大きいか(原点から見て)
	bool operator < (const Vector3& right);
	bool operator > (const Vector3& right);

	//内積
	static const float Dot(const Vector3& v1, const Vector3& v2);
	const float Dot(const Vector3& v);

	//外積
	static const Vector3 Cross(const Vector3& v1, const Vector3& v2);
	const Vector3 Cross(const Vector3& other);

	//ベクトルの長さの二乗
	const float LengthSquare();
	static float LengthSquare(const Vector3& v1,const Vector3& v2);

	//ベクトルの長さ
	const float Length();
	static float Length(const Vector3& v1,const Vector3& v2);

	//内積を使用した角度の算出	度数法で返して欲しいのなら,最後にtrueを入れる
	static float DotAngle(Vector3 A, Vector3 B, bool is_deg = false);

	//外積を使用した角度の算出	度数法で返して欲しいのなら,最後にtrueを入れる
	static float CrossAngleXZ(Vector3 A, Vector3 B, bool is_deg = false);

	//正規化
	Vector3 Normalize();
	void Normalized();

	//重点
	static Vector3 Emphasis(const Vector3& ver1,const Vector3& ver2,const Vector3& ver3);

	//線形補完
	static Vector3 Lerp(const Vector3& start,const Vector3& end,float t);

	//法線ベクトルによるベクトルの反射
	static Vector3 Reflect(Vector3 vec,Vector3 normal);

	//ベクトルの等値判定
	static bool Equal(const Vector3& left,const Vector3& right, float dist) {
		return LengthSquare(left,right) < (dist * dist);
	}
};
