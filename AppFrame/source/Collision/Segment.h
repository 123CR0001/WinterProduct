#pragma once
#include"Vector3.h"

class Segment {
public:
	Segment() {
		start = Vector3(0, 0, 0);
		end = Vector3(0, 0, 0);
	}
	Segment(const Vector3& s ,const Vector3& e){
		start = s;
		end = e;
	}
	Segment(const Segment& seg) {
		*this = seg;
	}

	Vector3 start;
	Vector3 end;

	//線分のベクトル
	Vector3 Vector() {
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
	Vector3 LinearInterpolation(const float& t) {
		return start + Vector3(end - start) * t;
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
		if ((int)Vector3::LengthSquare(s1.start, s1.end) == (int)Vector3::LengthSquare(s2.start, s2.end)) {
			if (s1.start == s2.start || s1.start == s2.end) {
				if (s1.end == s2.start || s1.end == s2.end) {
					return true;
				}
			}
		}
		return false;
	}

};