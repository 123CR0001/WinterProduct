#pragma once
#include"../Collision/Polygon.h"
#include<unordered_map>
#include <vector>
#include"../Collision/Collision.h"

struct Node {
	Polygon3D* neighborPoly = nullptr;
	Vector3D pos;

	bool operator == (const Node& right) {
		return neighborPoly == right.neighborPoly;
	}
};

typedef std::unordered_map<Polygon3D*, std::vector<Node>> ConectPolygonMap;
class Navi {
public:
	Navi() {
	}
	~Navi() {
		MV1DeleteModel(_handle);
	}

	//ポリゴン出の最短経路
	bool BFS(
		Vector3D goalPos,
		Vector3D startPos,
		std::vector<Vector3D>& route,
		std::vector<Polygon3D*>* routePolygon = nullptr
	); 

	//Dxlibで読み込んだ3Dモデルのポリゴンの頂点座標だけ取り出す
	void GetPolygonData();

	//porygonsに格納されているポリゴン頂点座標からポリゴンがつながっているか調べる
	//ポリゴンの頂点同士の重なりで調べている
	void GetConectPolygonMap();


	Polygon3D* GetHitPolygon(Vector3D pos);

	int LoadModel(const char* fileName, const char* attachName) {
		_handle = MV1LoadModel(fileName);
		_attachIndex = MV1SearchFrame(_handle, attachName);

		return _handle;
	}

	int GetHandle() const{ return _handle; }
	int GetAttachIndex()const { return _attachIndex; }

	std::vector<Polygon3D>GetPolygon() { return _polys; }
private:
	//モデルのハンドル
	int _handle;
	int _attachIndex;

	//モデルから形成したポリゴンデータを格納する変数
	std::vector<Polygon3D>_polys;

	//ポリゴン一つごとに、隣接する複数のポリゴンのアドレスを登録する変数
	ConectPolygonMap _neighborPolyMap;
};