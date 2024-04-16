#include"Vector3.h"

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

bool Vector3::operator < (const Vector3& right) {
	return this->LengthSquare() < LengthSquare(Vector3(),right);
}
bool Vector3::operator > (const Vector3& right) {
	return this->LengthSquare() > LengthSquare(Vector3(), right);
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

float Vector3::Length(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1 - v2).Length();
}
float Vector3::LengthSquare(const Vector3& v1,const Vector3& v2) {
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
	//�@���x�N�g���̒�����0���Ɠ������o�܂���̂Œ��ӂ��Ă��������B

	//�x�N�g��A��B�̒������v�Z����
	//���ςƃx�N�g���������g����cos�Ƃ����߂�
	A.Normalized();
	B.Normalized();
	float cos_sita = Vector3::Dot(A, B);

	//cos�Ƃ���Ƃ����߂�

	float sita = acos(cos_sita);

	if (is_deg) { sita = sita * 180.0 / 3.141592635; }

	return sita;
}

float Vector3::CrossAngleXZ(Vector3 A, Vector3 B, bool is_deg) {
	//�@���x�N�g���̒�����0���Ɠ������o�܂���̂Œ��ӂ��Ă��������B
	Vector3 a(A.x, A.z, 0.f);
	Vector3 b(B.x, B.z, 0.f);
//�x�N�g��A��B�̒������v�Z����
//�O�ςƃx�N�g���������g����sin�Ƃ����߂�
	a.Normalized();
	b.Normalized();

	float sinsit = Vector3::Cross(a, b).z;

	//cos�Ƃ���Ƃ����߂�
	float sita = asin(sinsit);

	//���W�A���łȂ�0�`180�̊p�x�łق����ꍇ�̓R�����g�O��
	if (is_deg) { sita = sita * 180.0 / 3.141592635; }

	return sita;
}

Vector3 Vector3::Emphasis(const Vector3& ver1, const Vector3& ver2, const Vector3& ver3) {
	return (ver1 + ver2 + ver3) / 3;
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t) {
	return start + (end - start) * t;
}

Vector3 Vector3::Reflect(Vector3 vec, Vector3 normal) {
	return vec - normal * (2.0f * Vector3::Dot(vec, normal));
}