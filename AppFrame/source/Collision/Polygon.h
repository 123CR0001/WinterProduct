#pragma once
#include"Vector3.h"
#include"Segment.h"

class Polygon3D {
	public:
		Polygon3D(const Vector3& v1,const Vector3& v2,const Vector3& v3) {
			ver1 = v1;
			ver2 = v2;
			ver3 = v3;
		}

		/*Polygon3D(Polygon3D poly) {
			*this = poly;
		}*/
		
		//法線ベクトル
		const Vector3& NormalVector() {
			return Vector3(Vector3::Cross((ver2 - ver1), (ver3 - ver1))).Normalize();
		}

		//重点
		Vector3 Emphasis() {
			return Vector3::Emphasis(ver1, ver2, ver3);
		}

		void SetScale(const float& t) {
			Vector3 emphasis(this->Emphasis());
			ver1 = Vector3::LineInter(ver1, emphasis, t);
			ver2 = Vector3::LineInter(ver2, emphasis, t);
			ver3 = Vector3::LineInter(ver3, emphasis, t);
		}

		bool IsConect(Polygon3D& other, Segment* side = nullptr) {
			//1辺目
			if (Segment::IsSame(Segment(ver1, ver2), Segment(other.ver1, other.ver2))) {
				if (side){*side = Segment(ver1, ver2); }
				return true;
			}
			if (Segment::IsSame(Segment(ver1, ver2), Segment(other.ver2, other.ver3))) {
				if (side) { *side = Segment(ver1, ver2); }
				return true;
			}
			if (Segment::IsSame(Segment(ver1, ver2), Segment(other.ver3, other.ver1))) {
				if (side) { *side = Segment(ver1, ver2); }
				return true;
			}

			if (Segment::IsSame(Segment(ver2, ver1), Segment(other.ver1, other.ver2))) {
				if (side) { *side = Segment(ver2, ver1); }
				return true;
			}
			if (Segment::IsSame(Segment(ver2, ver1), Segment(other.ver2, other.ver3))) {
				if (side) { *side = Segment(ver2, ver1); }
				return true;
			}
			if (Segment::IsSame(Segment(ver2, ver1), Segment(other.ver3, other.ver1))) {
				if (side) { *side = Segment(ver2, ver1); }
				return true;
			}

			//2辺目
			if (Segment::IsSame(Segment(ver2, ver3), Segment(other.ver1, other.ver2))) {
				if (side) { *side = Segment(ver2, ver3); }
				return true;
			}
			if (Segment::IsSame(Segment(ver2, ver3), Segment(other.ver2, other.ver3))) {
				if (side) { *side = Segment(ver2, ver3); }
				return true;
			}
			if (Segment::IsSame(Segment(ver2, ver3), Segment(other.ver3, other.ver1))) {
				if (side) { *side = Segment(ver2, ver3); }
				return true;
			}
			if (Segment::IsSame(Segment(ver3, ver2), Segment(other.ver1, other.ver2))) {
				if (side) { *side = Segment(ver3, ver2); }
				return true;
			}
			if (Segment::IsSame(Segment(ver3, ver2), Segment(other.ver2, other.ver3))) {
				if (side) { *side = Segment(ver3, ver2); }
				return true;
			}
			if (Segment::IsSame(Segment(ver3, ver2), Segment(other.ver3, other.ver1))) {
				if (side) { *side = Segment(ver3, ver2); }
				return true;
			}

			//3辺目
			if (Segment::IsSame(Segment(ver3, ver1), Segment(other.ver1, other.ver2))) {
				if (side) { *side = Segment(ver3, ver1); }
				return true;
			}
			if (Segment::IsSame(Segment(ver3, ver1), Segment(other.ver2, other.ver3))) {
				if (side) { *side = Segment(ver3, ver1); }
				return true;
			}
			if (Segment::IsSame(Segment(ver3, ver1), Segment(other.ver3, other.ver1))) {
				if (side) { *side = Segment(ver3, ver1); }
				return true;
			}

			if (Segment::IsSame(Segment(ver1, ver3), Segment(other.ver1, other.ver2))) {
				if (side) { *side = Segment(ver1, ver3); }
				return true;
			}
			if (Segment::IsSame(Segment(ver1, ver3), Segment(other.ver2, other.ver3))) {
				if (side) { *side = Segment(ver1, ver3); }
				return true;
			}
			if (Segment::IsSame(Segment(ver1, ver3), Segment(other.ver3, other.ver1))) {
				if (side) { *side = Segment(ver1, ver3); }
				return true;
			}
			return false;
		}

		Vector3 ver1;
		Vector3 ver2;
		Vector3 ver3;
};
