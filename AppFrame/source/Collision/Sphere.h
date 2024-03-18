#pragma once
#include"Vector3.h"
class Sphere {
public:
	Sphere(const Vector3& center, const float radius)
	{
		this->center = center;
		this->radius = radius;
	}

	~Sphere(){}

	Vector3 center;
	float radius;
};