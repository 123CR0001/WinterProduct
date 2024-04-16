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

	//DxLib��VECTOR�\���݂̂����Ɏg�p�������̂�public
	float x, y, z;

	//�l�̑��
	const Vector3& operator = (const Vector3& right) {
		x = right.x;
		y = right.y;
		z = right.z;
		return *this;
	}

	//�x�N�g���̉��Z���
	const Vector3& operator += (const Vector3& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	//�x�N�g���̌��Z���
	const Vector3& operator -= (const Vector3& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	//�x�N�g���̏�Z���
	const Vector3& operator *= (const float right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	//�x�N�g���̏��Z���
	const Vector3& operator /= (const float right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	//�x�N�g���̉��Z
	friend Vector3 operator+(const Vector3& left, const Vector3& right) {
		return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
	}

	//�x�N�g���̌��Z
	friend Vector3 operator-(const Vector3& left, const Vector3& right) {
		return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	//�x�N�g���̏�Z
	friend Vector3 operator * (const Vector3& left, float right) {
		return Vector3(left.x * right, left.y * right, left.z * right);
	}

	//�x�N�g���̏��Z���
	friend Vector3 operator / (const Vector3& left, float right) {
		return Vector3(left.x / right, left.y / right, left.z / right);
	}

	//��̍��W����������
	bool operator == (const Vector3& right);

	//��̍��W���������Ȃ���
	bool operator != (const Vector3& right);

	//�ǂ���̃x�N�g���̕����傫����(���_���猩��)
	bool operator < (const Vector3& right);
	bool operator > (const Vector3& right);

	//����
	static const float Dot(const Vector3& v1, const Vector3& v2);
	const float Dot(const Vector3& v);

	//�O��
	static const Vector3 Cross(const Vector3& v1, const Vector3& v2);
	const Vector3 Cross(const Vector3& other);

	//�x�N�g���̒����̓��
	const float LengthSquare();
	static float LengthSquare(const Vector3& v1,const Vector3& v2);

	//�x�N�g���̒���
	const float Length();
	static float Length(const Vector3& v1,const Vector3& v2);

	//���ς��g�p�����p�x�̎Z�o	�x���@�ŕԂ��ė~�����̂Ȃ�,�Ō��true������
	static float DotAngle(Vector3 A, Vector3 B, bool is_deg = false);

	//�O�ς��g�p�����p�x�̎Z�o	�x���@�ŕԂ��ė~�����̂Ȃ�,�Ō��true������
	static float CrossAngleXZ(Vector3 A, Vector3 B, bool is_deg = false);

	//���K��
	Vector3 Normalize();
	void Normalized();

	//�d�_
	static Vector3 Emphasis(const Vector3& ver1,const Vector3& ver2,const Vector3& ver3);

	//���`�⊮
	static Vector3 Lerp(const Vector3& start,const Vector3& end,float t);

	//�@���x�N�g���ɂ��x�N�g���̔���
	static Vector3 Reflect(Vector3 vec,Vector3 normal);

	//�x�N�g���̓��l����
	static bool Equal(const Vector3& left,const Vector3& right, float dist) {
		return LengthSquare(left,right) < (dist * dist);
	}
};
