#pragma once
#include"../Collision/Polygon.h"
#include<unordered_map>
#include <vector>
#include"../Collision/Collision.h"

struct Node {
	Polygon3D* neighborPoly = nullptr;
	Vector3 pos;

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

	//�|���S���o�̍ŒZ�o�H
	bool BFS(
		const Vector3& goalPos,
		const Vector3& startPos,
		std::vector<Vector3>& route,
		std::vector<Polygon3D*>* routePolygon = nullptr
	); 

	//Dxlib�œǂݍ���3D���f���̃|���S���̒��_���W�������o��
	void GetPolygonData();

	//porygons�Ɋi�[����Ă���|���S�����_���W����|���S�����Ȃ����Ă��邩���ׂ�
	//�|���S���̒��_���m�̏d�Ȃ�Œ��ׂĂ���
	void GetConectPolygonMap();


	Polygon3D* GetHitPolygon(Vector3 pos);

	int LoadModel(const char* fileName, const char* attachName) {
		_handle = MV1LoadModel(fileName);
		_attachIndex = MV1SearchFrame(_handle, attachName);

		return _handle;
	}

	int GetHandle() const{ return _handle; }
	int GetAttachIndex()const { return _attachIndex; }

	std::vector<Polygon3D>GetPolygon() { return _polys; }
private:
	//���f���̃n���h��
	int _handle;
	int _attachIndex;

	//���f������`�������|���S���f�[�^���i�[����ϐ�
	std::vector<Polygon3D>_polys;

	//�|���S������ƂɁA�אڂ��镡���̃|���S���̃A�h���X��o�^����ϐ�
	ConectPolygonMap _neighborPolyMap;
};