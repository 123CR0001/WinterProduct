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
	Vector3& operator *= (const float right) {
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
	const Vector3 operator + (const Vector3& right);

	//ベクトルの減算
	const Vector3 operator - (const Vector3& right);

	//ベクトルの乗算
	const Vector3 operator * (const float right);

	//ベクトルの除算代入
	const Vector3 operator / (const float right);

	//二つの座標が等しいか
	bool operator == (const Vector3& right);

	//二つの座標が等しくないか
	bool operator != (const Vector3& right);

	//どちらのベクトルの方が大きいか(原点から見て)
	bool operator < (Vector3 right);
	bool operator > (Vector3 right);

	//内積
	static const float Dot(const Vector3& v1, const Vector3& v2);
	const float Dot(const Vector3& v);

	//外積
	static const Vector3 Cross(const Vector3& v1, const Vector3& v2);
	const Vector3 Cross(const Vector3& other);

	//ベクトルの長さの二乗
	const float LengthSquare();
	static float LengthSquare(Vector3 v1, Vector3 v2);

	//ベクトルの長さ
	const float Length();
	static float Length(Vector3 v1, Vector3 v2);

	//内積を使用した角度の算出	度数法で返して欲しいのなら,最後にtrueを入れる
	static float DotAngle(Vector3 A, Vector3 B, bool is_deg = false);

	//外積を使用した角度の算出	度数法で返して欲しいのなら,最後にtrueを入れる
	static float CrossAngleXZ(Vector3 A, Vector3 B, bool is_deg = false);

	//正規化
	Vector3 Normalize();
	void Normalized();

	//重点
	static Vector3 Emphasis(Vector3 ver1, Vector3 ver2, Vector3 ver3);

	//線形補完
	static Vector3 LineInter( Vector3 start,Vector3 end, const float t);

	//法線ベクトルによるベクトルの反射
	static Vector3 Reflect(Vector3 vec,Vector3 normal);

	bool Equal(const Vector3& right, float dist) {
		return Vector3((*this) - right).LengthSquare() < (dist * dist);
	}
};
