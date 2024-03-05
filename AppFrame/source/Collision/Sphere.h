#pragma once
#include"Vector3D.h"
class Sphere {
public:
	Sphere(const Vector3D& center, const float radius)
	{
		this->center = center;
		this->radius = radius;
	}

	~Sphere(){}

	Vector3D center;
	float radius;
};