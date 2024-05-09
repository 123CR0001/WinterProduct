#pragma once
#include"Vector3.h"

class Plane3D {
public:
	Plane3D()
		:a(0.f)
		,b(0.f)
		,c(0.f)
		,d(0.f)
	{
	}
	Plane3D(float a, float b, float c, float d){
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}

	Plane3D(Vector3 p,Vector3 normal) {
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = p.Dot(normal);
	}
	float a, b, c, d;
	Vector3 GetP() {
		return Vector3(a * d, b * d, c * d);
	}
};