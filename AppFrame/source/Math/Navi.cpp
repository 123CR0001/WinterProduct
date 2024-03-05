#include"Navi.h"
#include"segment.h"
#include <queue>
#include <iostream>
#include <algorithm>
#include <limits>


bool Navi::BFS(
	Polygon3D* goal,
	Polygon3D* start,
	std::vector<Vector3D>& route,
	std::vector<Polygon3D*>* routePolygon
) {
	bool pathFound = false;

	if(goal == start) {
		return false;
	}

	std::queue<Polygon3D*> data;

	std::unordered_map<Polygon3D*, Node> routeCandi;

	data.emplace(start);

	while(!data.empty()) {

		Polygon3D* current = data.front();
		data.pop();

		if(current == goal)
		{
			Polygon3D* m = goal;
			std::vector<Vector3D> impuritiesMix;

			while(1) {

				if(!routeCandi[m].neighborPoly) { break; }

				impuritiesMix.emplace_back(routeCandi[m].pos);

				if(routePolygon) { routePolygon->emplace_back(routeCandi[m].neighborPoly); }

				m = routeCandi[m].neighborPoly;
			}

			route = impuritiesMix;

			pathFound = true;
			break;
		}

		auto conect = _neighborPolyMap[current];

		for(auto&& node : conect) {

			const Polygon3D* parent = routeCandi[node.neighborPoly].neighborPoly;

			if(parent == nullptr && node.neighborPoly != start) {
				routeCandi[node.neighborPoly].neighborPoly = current;
				routeCandi[node.neighborPoly].pos = node.pos;
				data.emplace(node.neighborPoly);
			}

		}
	}

	return pathFound;
}

void Navi::GetPolygonData() {
	//���f���Ɋ܂܂����������Ă���
	MV1_REF_POLYGONLIST list = MV1GetReferenceMesh(_handle, _attachIndex, TRUE);

	//���_�̃C���f�b�N�X
	MV1_REF_POLYGON* DxPolygon = list.Polygons;
	//���_�̈ʒu���
	MV1_REF_VERTEX* ver = list.Vertexs;

	//�|���S������\�z
	for (int a = 0; a < list.PolygonNum; a++) {
		Polygon3D myPolygon(
			DxConverter::DxToVec(ver[DxPolygon[a].VIndex[0]].Position),
			DxConverter::DxToVec(ver[DxPolygon[a].VIndex[1]].Position),
			DxConverter::DxToVec(ver[DxPolygon[a].VIndex[2]].Position)
		);
		_polys.emplace_back(myPolygon);
	}

}

void Navi::GetConectPolygonMap() {
	//�|���S�����m���אڂ��Ă��邩

	for(auto&& poly1 : _polys) {
		for(auto&& poly2 : _polys) {
			if(&poly1 == &poly2) { continue; }

			std::vector<Vector3D>vers;

			if(Collision::SegPointDistSq(poly2.ver1, Segment(poly1.ver1, poly1.ver2)) < 0.01) {
				vers.emplace_back(poly2.ver1);
			}
			else if(Collision::SegPointDistSq(poly2.ver1, Segment(poly1.ver2, poly1.ver3)) < 0.01) {
				vers.emplace_back(poly2.ver1);
			}
			else if(Collision::SegPointDistSq(poly2.ver1, Segment(poly1.ver3, poly1.ver1)) < 0.01) {
				vers.emplace_back(poly2.ver1);
			}

			//
			if(Collision::SegPointDistSq(poly2.ver2, Segment(poly1.ver1, poly1.ver2)) < 0.01) {
				vers.emplace_back(poly2.ver2);
			}
			else if(Collision::SegPointDistSq(poly2.ver2, Segment(poly1.ver2, poly1.ver3)) < 0.01) {
				vers.emplace_back(poly2.ver2);
			}
			else if(Collision::SegPointDistSq(poly2.ver2, Segment(poly1.ver3, poly1.ver1)) < 0.01) {
				vers.emplace_back(poly2.ver2);
			}

			//
			if(Collision::SegPointDistSq(poly2.ver3, Segment(poly1.ver1, poly1.ver2)) < 0.01) {
				vers.emplace_back(poly2.ver3);
			}
			else if(Collision::SegPointDistSq(poly2.ver3, Segment(poly1.ver2, poly1.ver3)) < 0.01) {
				vers.emplace_back(poly2.ver3);
			}
			else if(Collision::SegPointDistSq(poly2.ver3, Segment(poly1.ver3, poly1.ver1)) < 0.01) {
				vers.emplace_back(poly2.ver3);
			}

			/////////////////////
			if(Collision::SegPointDistSq(poly1.ver1, Segment(poly2.ver1, poly2.ver2)) < 0.01) {
				vers.emplace_back(poly1.ver1);
			}
			else if(Collision::SegPointDistSq(poly1.ver1, Segment(poly2.ver2, poly2.ver3)) < 0.01) {
				vers.emplace_back(poly1.ver1);
			}
			else if(Collision::SegPointDistSq(poly1.ver1, Segment(poly2.ver3, poly2.ver1)) < 0.01) {
				vers.emplace_back(poly1.ver1);
			}

			//
			if(Collision::SegPointDistSq(poly1.ver2, Segment(poly2.ver1, poly2.ver2)) < 0.01) {
				vers.emplace_back(poly1.ver2);
			}
			else if(Collision::SegPointDistSq(poly1.ver2, Segment(poly2.ver2, poly2.ver3)) < 0.01) {
				vers.emplace_back(poly1.ver2);
			}
			else if(Collision::SegPointDistSq(poly1.ver2, Segment(poly2.ver3, poly2.ver1)) < 0.01) {
				vers.emplace_back(poly1.ver2);
			}

			//
			if(Collision::SegPointDistSq(poly1.ver3, Segment(poly2.ver1, poly2.ver2)) < 0.01) {
				vers.emplace_back(poly1.ver3);
			}
			else if(Collision::SegPointDistSq(poly1.ver3, Segment(poly2.ver2, poly2.ver3)) < 0.01) {
				vers.emplace_back(poly1.ver3);
			}
			else if(Collision::SegPointDistSq(poly1.ver3, Segment(poly2.ver3, poly2.ver1)) < 0.01) {
				vers.emplace_back(poly1.ver3);
			}


			//vers��size()��2�ȊO���ƁA���_��������d�Ȃ��Ă������Ȃ�����
			if(vers.size() < 2) { continue; }

			//���_���́A���Ԃɕ���ł��邽�߁A�ŏ��ƍŌ�̒��_���͑������ƒ��ׂ��Ȃ��B�̂ŁA�����O�ɂ���Ă���
			float longestDistSq = Vector3D::LengthSquare(vers.front(), vers.back());

			Vector3D pos = Vector3D::LineInter(vers.front(), vers.back(), 0.5f);

			for(int a = 0; a < vers.size() - 1; a++) {

				float distSq = Vector3D::LengthSquare(vers[a], vers[a + 1]);

				if(distSq > longestDistSq) {
					longestDistSq = distSq;
					pos = Vector3D::LineInter(vers[a], vers[a + 1], 0.5f);
				}

			}

			if(longestDistSq > 0.001f) {

				Node node = { &poly2,pos };
				Node node2 = { &poly1,pos };

				if(std::find(_neighborPolyMap[&poly1].begin(), _neighborPolyMap[&poly1].end(), node) == _neighborPolyMap[&poly1].end()) {
					_neighborPolyMap[&poly1].emplace_back(node);
				}
				if(std::find(_neighborPolyMap[&poly2].begin(), _neighborPolyMap[&poly2].end(), node2) == _neighborPolyMap[&poly2].end()) {
					_neighborPolyMap[&poly2].emplace_back(node2);
				}

			}

		}
	}

}

Polygon3D* Navi::GetHitPoygon(Vector3D pos) {

	for (auto& poly : _polys) {
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
	//�Ȃ�������
	return nullptr;
}
