#pragma once
#include"Vector3D.h"

class Segment {
public:
	Segment() {
		start = Vector3D(0, 0, 0);
		end = Vector3D(0, 0, 0);
	}
	Segment(const Vector3D& s ,const Vector3D& e){
		start = s;
		end = e;
	}
	Segment(const Segment& seg) {
		*this = seg;
	}

	Vector3D start;
	Vector3D end;

	//線分のベクトル
	Vector3D Vector() {
		return end - start;
	}

	//線分のベクトルの長さの2乗
	float LengthSquare() {
		return this->Vector().LengthSquare();
	}

	//線分のベクトルの長さ
	float Length() {
		return this->Vector().Length();
	}

	/*線形補完　0.0 ~ 1.0 百分率*/
	Vector3D LinearInterpolation(const float& t) {
		return start + Vector3D(end - start) * t;
	}

	static bool  IsSame(Segment s1, Segment s2) {
		//if ((int)Vector3D::LengthSquare(s1.start, s1.end) == (int)Vector3D::LengthSquare(s2.start, s2.end)) {
			if (s1.start == s2.start && s1.end == s2.end) {
			
				return true;
				
			}
		//}
		return false;
	}

	static bool  IsOverLap(Segment s1, Segment s2) {
		if ((int)Vector3D::LengthSquare(s1.start, s1.end) == (int)Vector3D::LengthSquare(s2.start, s2.end)) {
			if (s1.start == s2.start || s1.start == s2.end) {
				if (s1.end == s2.start || s1.end == s2.end) {
					return true;
				}
			}
		}
		return false;
	}

};