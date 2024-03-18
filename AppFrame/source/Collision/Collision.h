#pragma once
#include"Capsule.h"
#include"Polygon.h"
#include"DxLib.h"
#include"Plane3D.h"
#include"Sphere.h"
#include"Vector2.h"
#include"Transform2.h"

constexpr auto zero_com = 0.00001;

//参考サイト


class DxConverter {
public:
	static VECTOR VecToDx(Vector3 v1) {
		return VGet(v1.x, v1.y, v1.z);
	}

	static Vector3 DxToVec(VECTOR v) {
		return Vector3(v.x, v.y,v.z);
	}

};

class Collision {
public:
	static void clamp01(float& v) {
		if (v < 0.0)
			v = 0.0;
		else if (v > 1.0)
			v = 1.0;
	}

	static const Vector3 SegPointLatestPoint(Vector3 p, Segment s, float* t = nullptr) {

		Vector3 v1 = p - s.start;
		Vector3 v2 = s.end - s.start;

		float v = v1.Dot(v2) / v2.Dot(v2);

		Vector3 result(s.start + v2 * (v));

		if (v1.Dot(v2) < 0.0) {
			result = s.start;
		}
		else if (Vector3::Dot(p - s.end, s.start - s.end) < 0.0) {
			result = s.end;
		}
		if (t) { *t = v; }

		return result;
	}

	static const float SegPointDistSq(Vector3 p, Segment s, float* t = nullptr) {

		Vector3 result(SegPointLatestPoint(p, s, t));

		return Vector3::LengthSquare(result, p);
	}

	static const float SegPointDist(Vector3 p, Segment s ,float* t = nullptr) {

		Vector3 result(SegPointLatestPoint(p,s,t));

		return Vector3::Length(result, p);
	}

	static bool IsParallel(Segment seg1, Segment seg2) {

		Vector3 v1(seg1.end - seg1.start);
		Vector3 v2(seg2.end - seg2.start);

		Vector3 c = v1.Cross(v2);

		float d = c.Length();

		return -zero_com < d && d < zero_com;
	}

	static float SegPointCoefficient(Vector3 p, Segment s) {
		Vector3 v1 = p - s.start;
		Vector3 v2 = s.end - s.start;

		return v1.Dot(v2) / v2.Dot(v2);
	}
	static float LineLineDistSq(Segment s1, Segment s2, float* t1 = nullptr, float* t2 = nullptr, Segment* r = nullptr) {


		Vector3 v1 = s1.Vector();
		Vector3 v2 = s2.Vector();

		float DV1V2 = v1.Dot(v2);
		float DV1V1 = v1.Dot(v1);
		float DV2V2 = v2.Dot(v2);

		Vector3 P21P11 = s1.start - s2.start;

		float T1 = (DV1V2 * v2.Dot(P21P11) - DV2V2 * v1.Dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);

		*t1 = (DV1V2 * v2.Dot(P21P11) - DV2V2 * v1.Dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
		Vector3 p1 = s1.start + (v1 * T1);
		float T2 = v2.Dot(p1 - s2.start);
		Vector3 p2 = s2.start + (v2 * T2);


		if (r) { *r = Segment(p1, p2); }
		if (t1) { *t1 = T1; }
		if (t2) { *t2 = T2; }

		return Vector3(p2 - p1).LengthSquare();

	}

	static float LineLineDist(Segment s1, Segment s2,float* t1 = nullptr,float* t2 = nullptr ,Segment* r = nullptr) {

		return sqrt(LineLineDistSq(s1, s2,t1,t2,r ));

	}

	//s1の線分上にある最短点がrの始点
	//s2の線分上にある最短点がrの終点
	static float SegSegDistSq(Segment s1, Segment s2, Segment* r = nullptr, float* t1 = nullptr, float* t2 = nullptr) {

		float T1 = 0.0f;
		float T2 = 0.0f;

		Segment result;

		if (Collision::IsParallel(s1, s2)) {
			T1 = 0.0;
			float len = SegPointDistSq(s1.start, s2,&T2);
			result.start = s1.start;
			result.end = s2.start + s2.Vector() * T2;
			if (0.0 <= T2 && T2 <= 1.0) {
				if (t1) { *t1 = T1; }
				if (t2) { *t2 = T2; }
				if (r) { *r = result; }
				return len;
			}
		}
		else {
			float len = LineLineDistSq(s1, s2, &T1, &T2,&result);

			if (
				0.0f <= T1 && T1 <= 1.0f &&
				0.0f <= T2 && T2 <= 1.0f
				) {
				if (t1) { *t1 = T1; }
				if (t2) { *t2 = T2; }
				if (r) { *r = result; }
				return len;
			}
		}

		Vector3 v1(s1.Vector());
		Vector3 v2(s2.Vector());

		clamp01(T1);
		result.start = s1.start + (v1 * T1);
		float len = SegPointDistSq(result.start, s2, &T2);

		if (0.0f <= T2 && T2 <= 1.0f) {

			result.end = s2.start + s2.Vector() * T2;
			if (t1) { *t1 = T1; }
			if (t2) { *t2 = T2; }
			if (r) { *r = result; }
			return len;
		}

		// S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
		clamp01(T2);
		result.end = s2.start + (v2* T2);
		len = SegPointDistSq(result.end, s1, &T1);
		if (0.0f <= T1 && T1 <= 1.0f) {

			result.start = s1.start + s1.Vector() * T1;
			if (t1) { *t1 = T1; }
			if (t2) { *t2 = T2; }
			if (r) { *r = result; }
			return len;
		}

		// 双方の端点が最短と判明
		clamp01(T1);
		result.start = s1.start + (v1 * T1);
		if (t1) { *t1 = T1; }
		if (t2) { *t2 = T2; }
		if (r) { *r = result; }
		return result.LengthSquare();
	}

	static float SegSegDist(Segment s1, Segment s2, Segment* r = nullptr, float* t1 = nullptr, float* t2 = nullptr) {
		return sqrt(SegSegDistSq(s1, s2, r, t1, t2));
	}

	static float PointPolygonDist(Polygon3D pol, Vector3 point) {
		float d = -Vector3::Dot(pol.NormalVector(), pol.ver1);

		return Vector3::Dot(point, pol.NormalVector()) - d;
	}

	static float SegPolygonDistSq(Segment s, Polygon3D pol) {
		return Segment_Triangle_MinLength_Square(
		DxConverter::VecToDx(s.start),
		DxConverter::VecToDx(s.end),
		DxConverter::VecToDx(pol.ver1),
		DxConverter::VecToDx(pol.ver2),
		DxConverter::VecToDx(pol.ver3)
		);
	}

	static float SegPolygonDist(Segment s, Polygon3D pol) {
		return sqrt(SegPolygonDistSq(s, pol));
	}

	static Vector3 PointPolygonVec(Polygon3D pol, Vector3 p) {
		return DxConverter::DxToVec(Plane_Point_MinLength_Position(
			DxConverter::VecToDx(pol.ver1),
			DxConverter::VecToDx(Vector3(pol.NormalVector()).Normalize()),
			DxConverter::VecToDx(p)
		));
	}

	static Plane3D PolToPlane(Polygon3D pol) {
		Vector3 ab = pol.ver2 - pol.ver1;
		Vector3 bc = pol.ver3 - pol.ver2;

		Vector3 normal = Vector3::Cross(ab, bc);
		
		return Plane3D(pol.ver1, normal.Normalize());
	}

	static float PolyToPointDist(Vector3 A, Vector3 P, Vector3 N);

	static Vector3 NearPosOnPlane(Vector3 A, Vector3 P, Vector3 N)
	{
		//PAベクトル(A-P)
		Vector3 PA(A-P);

		//法線NとPAを内積
		//法線の順方向に点Aがあればd > 0、 逆方向だとd < 0
		Vector3 normal(N.Normalize());

		float d = Vector3::Dot(normal, PA);

		//内積値から平面上の最近点を求める
		normal *= d;
		Vector3 ret(A - normal);

		return ret;
	}

	static bool IsHitPointPolygon(Polygon3D pol, Vector3 P) {

		//点と三角形は同一平面上にあるものとしています。同一平面上に無い場合は正しい結果になりません
		//線上は外とみなします。
		//ABCが三角形かどうかのチェックは省略...

		Vector3 AB = pol.ver2 - pol.ver1;
		Vector3 BP = P - pol.ver2;

		Vector3 BC = pol.ver3 - pol.ver2;
		Vector3 CP = P - pol.ver3;
		Vector3 CA = pol.ver1- pol.ver3;
		Vector3 AP = P - pol.ver1;

		Vector3 c1 = Vector3::Cross(AB, BP);
		Vector3 c2 = Vector3::Cross(BC, CP);
		Vector3 c3 = Vector3::Cross(CA, AP);


		//内積で順方向か逆方向か調べる
		double dot_12 = Vector3::Dot(c1, c2);
		double dot_13 = Vector3::Dot(c1, c3);

		if (dot_12 > 0 && dot_13 > 0) {
			//三角形の内側に点がある
			return true;
		}

		//三角形の外側に点がある
		return false;
	}

	static Vector3 PolygonPointLatestPoint(Polygon3D pol, Vector3 p) {

		Vector3 result(Collision::NearPosOnPlane(p, Collision::PolToPlane(pol).GetP(), Vector3(pol.NormalVector()).Normalize()));

		if (!IsHitPointPolygon(pol, result)){
			Segment s[3] = { Segment(pol.ver1, pol.ver2),Segment(pol.ver2, pol.ver3),Segment(pol.ver3, pol.ver1) };

			double dis = 99999999999;

			for (int a = 0; a < 3; a++) {
				double old_dis = dis;
				double t = 0;
				dis = SegPointDist(p, s[a]);
				if (old_dis > dis) {
					result = SegPointLatestPoint(p,s[a]);
				}
				else {
					dis = old_dis;
				}
			}
		}

		return result;

	}

	static SEGMENT_TRIANGLE_RESULT SegmentPolygonAnalyze(Polygon3D pol, Segment s) {
		VECTOR ss = DxConverter::VecToDx(s.start);
		VECTOR se = DxConverter::VecToDx(s.end);
		VECTOR pol1 = DxConverter::VecToDx(pol.ver1);
		VECTOR pol2 = DxConverter::VecToDx(pol.ver2);
		VECTOR pol3 = DxConverter::VecToDx(pol.ver3);
		SEGMENT_TRIANGLE_RESULT result;

		Segment_Triangle_Analyse(&ss, &se, &pol1, &pol2, &pol3, &result);

		return result;
	}

	//カプセル同士の当たり判定
	static bool Intersection(const Capsule& right, const Capsule& left,Segment* result = nullptr);
	static bool Intersection(const Polygon3D& poly, const Capsule& capsule);
	static bool Intersection(const Capsule& capsule, const Sphere& sphere);
};
