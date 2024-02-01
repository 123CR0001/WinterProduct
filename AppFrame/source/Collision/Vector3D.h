#pragma once
#include<cmath>

class Vector3D {
public:
	Vector3D() { x = 0.0f; y = 0.0f; z = 0.0f; }
	Vector3D(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3D(const Vector3D& other) {
		*this = other;
	}
	virtual ~Vector3D() {}

	//DxLibのVECTOR構造体みたいに使用したいのでpublic
	float x, y, z;

	//値の代入
	const Vector3D& operator = (const Vector3D& right) {
		x = right.x;
		y = right.y;
		z = right.z;
		return *this;
	}

	//ベクトルの加算代入
	const Vector3D& operator += (const Vector3D& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	//ベクトルの減算代入
	const Vector3D& operator -= (const Vector3D& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	//ベクトルの乗算代入
	const Vector3D& operator *= (const float& right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	//ベクトルの除算代入
	const Vector3D& operator /= (const float& right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	//ベクトルの加算
	const Vector3D operator + (const Vector3D& right);

	//ベクトルの減算
	const Vector3D operator - (const Vector3D& right);

	//ベクトルの乗算
	const Vector3D operator * (const float& right);

	//ベクトルの除算代入
	const Vector3D operator / (const float& right);

	//二つの座標が等しいか
	bool operator == (const Vector3D& right);

	//二つの座標が等しくないか
	bool operator != (const Vector3D& right);

	//どちらのベクトルの方が大きいか(原点から見て)
	bool operator < (Vector3D right);
	bool operator > (Vector3D right);

	//内積
	static const float Dot(const Vector3D& v1, const Vector3D& v2);
	const float Dot(const Vector3D& v);

	//外積
	static const Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
	const Vector3D Cross(const Vector3D& other);

	//ベクトルの長さの二乗
	const float LengthSquare();
	static float LengthSquare(Vector3D v1, Vector3D v2);

	//ベクトルの長さ
	const float Length();
	static float Length(Vector3D v1, Vector3D v2);

	//内積を使用した角度の算出	度数法で返して欲しいのなら,最後にtrueを入れる
	static float DotAngle(Vector3D A, Vector3D B, bool is_deg = false);

	//外積を使用した角度の算出	度数法で返して欲しいのなら,最後にtrueを入れる
	static float CrossAngleXZ(Vector3D A, Vector3D B, bool is_deg = false);

	//正規化
	Vector3D Normalize();
	void Normalized();

	//重点
	static Vector3D Emphasis(Vector3D ver1, Vector3D ver2, Vector3D ver3);

	//線形補完
	static Vector3D LineInter( Vector3D start,Vector3D end, const float& t);
};
