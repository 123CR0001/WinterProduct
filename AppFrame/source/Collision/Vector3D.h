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

	//DxLib��VECTOR�\���݂̂����Ɏg�p�������̂�public
	float x, y, z;

	//�l�̑��
	const Vector3D& operator = (const Vector3D& right) {
		x = right.x;
		y = right.y;
		z = right.z;
		return *this;
	}

	//�x�N�g���̉��Z���
	const Vector3D& operator += (const Vector3D& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	//�x�N�g���̌��Z���
	const Vector3D& operator -= (const Vector3D& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	//�x�N�g���̏�Z���
	const Vector3D& operator *= (const float& right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	//�x�N�g���̏��Z���
	const Vector3D& operator /= (const float& right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	//�x�N�g���̉��Z
	const Vector3D operator + (const Vector3D& right);

	//�x�N�g���̌��Z
	const Vector3D operator - (const Vector3D& right);

	//�x�N�g���̏�Z
	const Vector3D operator * (const float& right);

	//�x�N�g���̏��Z���
	const Vector3D operator / (const float& right);

	//��̍��W����������
	bool operator == (const Vector3D& right);

	//��̍��W���������Ȃ���
	bool operator != (const Vector3D& right);

	//�ǂ���̃x�N�g���̕����傫����(���_���猩��)
	bool operator < (Vector3D right);
	bool operator > (Vector3D right);

	//����
	static const float Dot(const Vector3D& v1, const Vector3D& v2);
	const float Dot(const Vector3D& v);

	//�O��
	static const Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
	const Vector3D Cross(const Vector3D& other);

	//�x�N�g���̒����̓��
	const float LengthSquare();
	static float LengthSquare(Vector3D v1, Vector3D v2);

	//�x�N�g���̒���
	const float Length();
	static float Length(Vector3D v1, Vector3D v2);

	//���ς��g�p�����p�x�̎Z�o	�x���@�ŕԂ��ė~�����̂Ȃ�,�Ō��true������
	static float DotAngle(Vector3D A, Vector3D B, bool is_deg = false);

	//�O�ς��g�p�����p�x�̎Z�o	�x���@�ŕԂ��ė~�����̂Ȃ�,�Ō��true������
	static float CrossAngleXZ(Vector3D A, Vector3D B, bool is_deg = false);

	//���K��
	Vector3D Normalize();
	void Normalized();

	//�d�_
	static Vector3D Emphasis(Vector3D ver1, Vector3D ver2, Vector3D ver3);

	//���`�⊮
	static Vector3D LineInter( Vector3D start,Vector3D end, const float& t);
};
