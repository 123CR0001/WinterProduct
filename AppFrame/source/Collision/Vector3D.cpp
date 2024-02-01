#include"Vector3D.h"

const Vector3D Vector3D::operator + (const Vector3D& right) {
	return Vector3D(*this) += right;
}

const Vector3D Vector3D::operator - (const Vector3D& right) {
	return Vector3D(*this) -= right;
}

const Vector3D Vector3D::operator * (const float& right) {
	return Vector3D(*this) *= right;
}

const Vector3D Vector3D::operator / (const float& right) {
	return Vector3D(*this) /= right;
}

bool Vector3D::operator == (const Vector3D& right) {
	if (fabsf(x - right.x) > 0.01f) { return false; }
	if (fabsf(y - right.y) > 0.01f) { return false; }
	if (fabsf(z - right.z) > 0.01f) { return false; }
	return true;
}

bool Vector3D::operator != (const Vector3D& right) {
	if (fabs(x - right.x) > 0.0) { return true; }
	if (fabs(y - right.y) > 0.0) { return true; }
	if (fabs(z - right.z) > 0.0) { return true; }
	return false;
}

bool Vector3D::operator < ( Vector3D right) {
	return this->LengthSquare() < right.LengthSquare();
}
bool Vector3D::operator > (Vector3D right) {
	return this->LengthSquare() > right.LengthSquare();
}

const float Vector3D::Dot(const Vector3D& v1, const Vector3D& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

const float Vector3D::Dot(const Vector3D& v) {
	return Dot(*this, v);
}

const float Vector3D::Length() {
	return sqrt(Dot(*this, *this));
}

float Vector3D::Length(Vector3D v1, Vector3D v2) {
	return Vector3D(v1 - v2).Length();
}
float Vector3D::LengthSquare(Vector3D v1, Vector3D v2) {
	return Vector3D(v1 - v2).LengthSquare();
}
const float Vector3D::LengthSquare() {
	return Dot(*this, *this);
}

Vector3D Vector3D::Normalize(){
	return *this / Length();
}

void Vector3D::Normalized() {
	*this /=  Length();
}

const Vector3D Vector3D::Cross(const Vector3D& v1, const Vector3D& v2) {
	Vector3D result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

const Vector3D Vector3D::Cross(const Vector3D& other) {
	return Cross(*this, other);
}

float Vector3D::DotAngle(Vector3D A, Vector3D B, bool is_deg) {
	//　※ベクトルの長さが0だと答えが出ませんので注意してください。

	//ベクトルAとBの長さを計算する
	//内積とベクトル長さを使ってcosθを求める
	A.Normalized();
	B.Normalized();
	float cos_sita = Vector3D::Dot(A, B);

	//cosθからθを求める

	float sita = acos(cos_sita);

	if (is_deg) { sita = sita * 180.0 / 3.141592635; }

	return sita;
}

float Vector3D::CrossAngleXZ(Vector3D A, Vector3D B, bool is_deg) {
	//　※ベクトルの長さが0だと答えが出ませんので注意してください。
	Vector3D a(A.x, A.z, 0.f);
	Vector3D b(B.x, B.z, 0.f);
//ベクトルAとBの長さを計算する
//外積とベクトル長さを使ってsinθを求める
	a.Normalized();
	b.Normalized();

	float sinsit = Vector3D::Cross(a, b).z;

	//cosθからθを求める
	float sita = asin(sinsit);

	//ラジアンでなく0～180の角度でほしい場合はコメント外す
	if (is_deg) { sita = sita * 180.0 / 3.141592635; }

	return sita;
}

Vector3D Vector3D::Emphasis(Vector3D ver1, Vector3D ver2, Vector3D ver3) {
	return Vector3D(ver1 + (ver2 + ver3)) / 3;
}

Vector3D Vector3D::LineInter(Vector3D start, Vector3D end, const float& t) {
	return start + (Vector3D(end - start) * t);
}