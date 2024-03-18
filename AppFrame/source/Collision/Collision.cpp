#include"Collision.h"

float Collision::PolyToPointDist(Vector3 A, Vector3 P, Vector3 N) {
	//PA�x�N�g��(A-P)
	Vector3 PA(A - P);
	return abs(PA.Dot(N.Normalize()));
}

bool Collision::Intersection(const Capsule& right, const Capsule& left, Segment* result) {
	return SegSegDistSq(right.seg, left.seg, result) < (right.radius + left.radius) * (right.radius + left.radius);
}

bool Collision::Intersection(const Polygon3D& poly, const Capsule& capsule) {
	return false;
}

bool Collision::Intersection(const Capsule& capsule, const Sphere& sphere) {
	return SegPointDistSq(sphere.center, capsule.seg) < (capsule.radius + sphere.radius) * (capsule.radius + sphere.radius);
}