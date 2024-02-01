#include"Navi.h"
#include <queue>
#include <iostream>
#include <algorithm>
#include <limits>


bool Navi::BFS(
	ConectPolygonMap& list,
	Polygon3D* goal,
	Polygon3D* start,
	std::vector<Vector3D>& route,
	std::vector<Polygon3D*>* routePolygon
) {
	bool pathFound = false;

	if (goal == start) {
		return false;
	}

	std::queue<Polygon3D*> data;

	std::unordered_map<Polygon3D*, Polygon3D*> routeCandi;

	data.emplace(start);

	while (!data.empty()) {

		Polygon3D* current = data.front();
		data.pop();

		if (current == goal)
		{
			Polygon3D* m = goal;
			std::vector<Vector3D> impuritiesMix;

			while (1) {

				if (!routeCandi[m]) { break; }

				Segment side;

				if (m->IsConect(*routeCandi[m], &side)) {
					//ポリゴンが重なっている辺の中央の位置を通るポイントとする
					impuritiesMix.emplace_back(side.LinearInterpolation(0.5));

				}

				if (routePolygon) { routePolygon->emplace_back(routeCandi[m]); }

				m = routeCandi[m];
			}

			route = impuritiesMix;

			pathFound = true;
			break;
		}

		auto conect = list[current];

		for (auto&& polygon : conect) {

			const Polygon3D* parent = routeCandi[polygon];

			if (parent == nullptr && polygon != start) {
				routeCandi[polygon] = current;
				data.emplace(polygon);
			}

		}
	}

	return pathFound;
}

void Navi::GetPolygonData(const int& handle, std::vector<Polygon3D>* polygons) {
	//モデルに含まれる情報を持ってくる
	MV1_REF_POLYGONLIST list = MV1GetReferenceMesh(handle, 0, TRUE);

	//頂点のインデックス
	MV1_REF_POLYGON* DxPolygon = list.Polygons;
	//頂点の位置情報
	MV1_REF_VERTEX* ver = list.Vertexs;

	//ポリゴン場を構築
	for (int a = 0; a < list.PolygonNum; a++) {
		Polygon3D myPolygon(
			DxConverter::DxToVec(ver[DxPolygon[a].VIndex[0]].Position),
			DxConverter::DxToVec(ver[DxPolygon[a].VIndex[1]].Position),
			DxConverter::DxToVec(ver[DxPolygon[a].VIndex[2]].Position)
		);
		polygons->emplace_back(myPolygon);
	}

}

void Navi::GetConectPolygonMap(std::vector<Polygon3D>& polygons, ConectPolygonMap& conectPolygonMap) {
	//ポリゴン同士が隣接しているか

	for (auto&& poly1 : polygons) {
		short count = 0;
		for (auto&& poly2 : polygons) {
			if (&poly1 == &poly2) { continue; }
			//if (Vector3D::LengthSquare(poly1.Emphasis(), poly2.Emphasis()) > 100 * 1.414 * 100 * 1.414) { continue; }
			if (poly1.IsConect(poly2)) {
				conectPolygonMap[&poly1].emplace_back(&poly2); count++;
			}
			if (count == 3) { break; }
		}
	}

}

Polygon3D* Navi::GetHitPoygon(Vector3D pos, std::vector<Polygon3D>& polygons) {

	for (auto& poly : polygons) {
		if (HitCheck_Line_Triangle(
			DxConverter::VecToDx(pos - Vector3D(0.f, 1000.f, 0.f)),
			DxConverter::VecToDx(pos + Vector3D(0.f, 40.f, 0.f)),
			DxConverter::VecToDx(poly.ver1),
			DxConverter::VecToDx(poly.ver2),
			DxConverter::VecToDx(poly.ver3)
		).HitFlag) {
			return &poly;
		}

	}
	//なかったら
	return nullptr;
}
