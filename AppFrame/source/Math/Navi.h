#pragma once
#include"../Collision/Polygon.h"
#include<unordered_map>
#include <vector>
#include"../Collision/Collision.h"

typedef std::unordered_map<Polygon3D*, std::vector<Polygon3D*>> ConectPolygonMap;

class Navi {
public:

	//�|���S���o�̍ŒZ�o�H
	static bool BFS(
		ConectPolygonMap& list,
		Polygon3D* goal,
		Polygon3D* start,
		std::vector<Vector3D>& route,
		std::vector<Polygon3D*>* routePolygon = nullptr
	); 

	//Dxlib�œǂݍ���3D���f���̃|���S���̒��_���W�������o��
	static void GetPolygonData(const int& handle, std::vector<Polygon3D>* polygons);

	//porygons�Ɋi�[����Ă���|���S�����_���W����|���S�����Ȃ����Ă��邩���ׂ�
	//�|���S���̒��_���m�̏d�Ȃ�Œ��ׂĂ���
	static void GetConectPolygonMap(std::vector<Polygon3D>& polygons, ConectPolygonMap& conectPolygonMap);

	static Polygon3D* GetHitPoygon(Vector3D pos, std::vector<Polygon3D>& polygons);
};