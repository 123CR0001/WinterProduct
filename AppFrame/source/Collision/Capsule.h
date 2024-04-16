#pragma once
#include"Segment.h"

class Capsule {
public:
	Capsule() {
		seg = Segment(Vector3(0,0,0), Vector3(0, 0, 0));
		radius = 0;
	}
	Capsule(const Segment& seg, float rad) {
		this->seg = seg;
		radius = rad;
	}
	Capsule(const Vector3& s,const Vector3& e, float rad) {
		seg = Segment(s,e);
		radius = rad;
	}
	Segment seg;
	float radius;
};
