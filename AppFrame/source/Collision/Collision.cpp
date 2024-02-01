#include"Collision.h"

float Collision::PolyToPointDist(Vector3D A, Vector3D P, Vector3D N) {
	//PAƒxƒNƒgƒ‹(A-P)
	Vector3D PA(A - P);
	return abs(PA.Dot(N.Normalize()));
}

bool Collision::Intersection(const Capsule& right, const Capsule& left, Segment* result) {
	return SegSegDistSq(right.seg, left.seg, result) < (right.radius + left.radius) * (right.radius + left.radius);
}

bool Collision::Intersection(const Polygon3D& poly, const Capsule& capsule) {
	return false;
}

