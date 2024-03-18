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
	Vector3& operator *= (const float right) {
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
	const Vector3 operator + (const Vector3& right);

	//�x�N�g���̌��Z
	const Vector3 operator - (const Vector3& right);

	//�x�N�g���̏�Z
	const Vector3 operator * (const float right);

	//�x�N�g���̏��Z���
	const Vector3 operator / (const float right);

	//��̍��W����������
	bool operator == (const Vector3& right);

	//��̍��W���������Ȃ���
	bool operator != (const Vector3& right);

	//�ǂ���̃x�N�g���̕����傫����(���_���猩��)
	bool operator < (Vector3 right);
	bool operator > (Vector3 right);

	//����
	static const float Dot(const Vector3& v1, const Vector3& v2);
	const float Dot(const Vector3& v);

	//�O��
	static const Vector3 Cross(const Vector3& v1, const Vector3& v2);
	const Vector3 Cross(const Vector3& other);

	//�x�N�g���̒����̓��
	const float LengthSquare();
	static float LengthSquare(Vector3 v1, Vector3 v2);

	//�x�N�g���̒���
	const float Length();
	static float Length(Vector3 v1, Vector3 v2);

	//���ς��g�p�����p�x�̎Z�o	�x���@�ŕԂ��ė~�����̂Ȃ�,�Ō��true������
	static float DotAngle(Vector3 A, Vector3 B, bool is_deg = false);

	//�O�ς��g�p�����p�x�̎Z�o	�x���@�ŕԂ��ė~�����̂Ȃ�,�Ō��true������
	static float CrossAngleXZ(Vector3 A, Vector3 B, bool is_deg = false);

	//���K��
	Vector3 Normalize();
	void Normalized();

	//�d�_
	static Vector3 Emphasis(Vector3 ver1, Vector3 ver2, Vector3 ver3);

	//���`�⊮
	static Vector3 LineInter( Vector3 start,Vector3 end, const float t);

	//�@���x�N�g���ɂ��x�N�g���̔���
	static Vector3 Reflect(Vector3 vec,Vector3 normal);

	bool Equal(const Vector3& right, float dist) {
		return Vector3((*this) - right).LengthSquare() < (dist * dist);
	}
};
