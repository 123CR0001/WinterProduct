#pragma once
#include"../Collision/Polygon.h"
#include<unordered_map>
#include <vector>
#include"../Collision/Collision.h"

typedef std::unordered_map<Polygon3D*, std::vector<Polygon3D*>> ConectPolygonMap;

class Navi {
public:

	//ポリゴン出の最短経路
	static bool BFS(
		ConectPolygonMap& list,
		Polygon3D* goal,
		Polygon3D* start,
		std::vector<Vector3D>& route,
		std::vector<Polygon3D*>* routePolygon = nullptr
	); 

	//Dxlibで読み込んだ3Dモデルのポリゴンの頂点座標だけ取り出す
	static void GetPolygonData(const int& handle, std::vector<Polygon3D>* polygons);

	//porygonsに格納されているポリゴン頂点座標からポリゴンがつながっているか調べる
	//ポリゴンの頂点同士の重なりで調べている
	static void GetConectPolygonMap(std::vector<Polygon3D>& polygons, ConectPolygonMap& conectPolygonMap);

	static Polygon3D* GetHitPoygon(Vector3D pos, std::vector<Polygon3D>& polygons);
};