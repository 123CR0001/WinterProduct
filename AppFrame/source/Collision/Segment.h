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

	//�����̃x�N�g��
	Vector3 Vector() {
		return end - start;
	}

	//�����̃x�N�g���̒�����2��
	float LengthSquare() {
		return this->Vector().LengthSquare();
	}

	//�����̃x�N�g���̒���
	float Length() {
		return this->Vector().Length();
	}

	/*���`�⊮�@0.0 ~ 1.0 �S����*/
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