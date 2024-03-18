#include"Vector3.h"

const Vector3 Vector3::operator + (const Vector3& right) {
	return Vector3(*this) += right;
}

const Vector3 Vector3::operator - (const Vector3& right) {
	return Vector3(*this) -= right;
}

const Vector3 Vector3::operator * (const float right) {
	return Vector3(*this) *= right;
}

const Vector3 Vector3::operator / (const float right) {
	return Vector3(*this) /= right;
}

bool Vector3::operator == (const Vector3& right) {
	if (fabsf(x - right.x) > 0.01f) { return false; }
	if (fabsf(y - right.y) > 0.01f) { return false; }
	if (fabsf(z - right.z) > 0.01f) { return false; }
	return true;
}

bool Vector3::operator != (const Vector3& right) {
	if (fabs(x - right.x) > 0.0) { return true; }
	if (fabs(y - right.y) > 0.0) { return true; }
	if (fabs(z - right.z) > 0.0) { return true; }
	return false;
}

bool Vector3::operator < ( Vector3 right) {
	return this->LengthSquare() < right.LengthSquare();
}
bool Vector3::operator > (Vector3 right) {
	return this->LengthSquare() > right.LengthSquare();
}

const float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

const float Vector3::Dot(const Vector3& v) {
	return Dot(*this, v);
}

const float Vector3::Length() {
	return sqrt(Dot(*this, *this));
}

float Vector3::Length(Vector3 v1, Vector3 v2) {
	return Vector3(v1 - v2).Length();
}
float Vector3::LengthSquare(Vector3 v1, Vector3 v2) {
	return Vector3(v1 - v2).LengthSquare();
}
const float Vector3::LengthSquare() {
	return Dot(*this, *this);
}

Vector3 Vector3::Normalize(){
	return *this / Length();
}

void Vector3::Normalized() {
	*this /=  Length();
}

const Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

const Vector3 Vector3::Cross(const Vector3& other) {
	return Cross(*this, other);
}

float Vector3::DotAngle(Vector3 A, Vector3 B, bool is_deg) {
	//　※ベクトルの長さが0だと答えが出ませんので注意してください。

	//ベクトルAとBの長さを計算する
	//内積とベクトル長さを使ってcosθを求める
	A.Normalized();
	B.Normalized();
	float cos_sita = Vector3::Dot(A, B);

	//cosθからθを求める

	float sita = acos(cos_sita);

	if (is_deg) { sita = sita * 180.0 / 3.141592635; }

	return sita;
}

float Vector3::CrossAngleXZ(Vector3 A, Vector3 B, bool is_deg) {
	//　※ベクトルの長さが0だと答えが出ませんので注意してください。
	Vector3 a(A.x, A.z, 0.f);
	Vector3 b(B.x, B.z, 0.f);
//ベクトルAとBの長さを計算する
//外積とベクトル長さを使ってsinθを求める
	a.Normalized();
	b.Normalized();

	float sinsit = Vector3::Cross(a, b).z;

	//cosθからθを求める
	float sita = asin(sinsit);

	//ラジアンでなく0～180の角度でほしい場合はコメント外す
	if (is_deg) { sita = sita * 180.0 / 3.141592635; }

	return sita;
}

Vector3 Vector3::Emphasis(Vector3 ver1, Vector3 ver2, Vector3 ver3) {
	return Vector3(ver1 + (ver2 + ver3)) / 3;
}

Vector3 Vector3::LineInter(Vector3 start, Vector3 end, const float t) {
	return start + (Vector3(end - start) * t);
}

Vector3 Vector3::Reflect(Vector3 vec, Vector3 normal) {
	return vec - normal * (2.0f * Vector3::Dot(vec, normal));
}