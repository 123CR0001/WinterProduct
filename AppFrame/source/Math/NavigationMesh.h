#pragma once
#pragma once
#include<unordered_map>
#include <vector>
#include"../Collision/Collision.h"

struct AdjacentData {
	Segment segment;
};

class NavigationMesh
{
public:
	NavigationMesh(const Polygon3D& mesh);
	~NavigationMesh();

	static NavigationMesh* FindPath(std::vector<NavigationMesh>& list, NavigationMesh* start, NavigationMesh* goal, float width = 0.f);

	NavigationMesh* GetParent() { return _parent; }

	void RegisterAdjacent(NavigationMesh* adjacent, const AdjacentData& data) {

		//���łɓo�^����Ă���ꍇ�͓o�^���Ȃ�
		for (auto&& a : _adjacentDataList) {
			if (a.first == adjacent) { return; }
		}

		_adjacentDataList.emplace_back(adjacent, data);
	}

	void SetBlocked(bool blocked) { _Blocked = blocked; }

	static void CreateList(const std::vector<Polygon3D>& _polygons, std::vector<NavigationMesh>& outList);

	Polygon3D GetMesh()const { return _mesh; }

	std::vector<std::pair<NavigationMesh*, AdjacentData>> GetAdjacent()& { return _adjacentDataList; }

	void Clear() {
		_InOpenSet = false;
		_InClosedSet = false;
		_parent = nullptr;
		_cost = 0.f;
	}

	float GetCost()const { return _cost; }

	const Vector3& GetPosition()const { return _position; }
private:
	//�אڂ���|���S��
	std::vector<std::pair<NavigationMesh*, AdjacentData>>_adjacentDataList;

	//�e
	NavigationMesh* _parent;

	//���b�V��
	Polygon3D _mesh;

	//�I�[�v���Z�b�g
	bool _InOpenSet;
	//�N���[�[�b�g
	bool _InClosedSet;

	//�i�r���b�V��(��)�Ƃ��Ďg�p�ł��邩
	bool _Blocked;

	//�R�X�g
	float _cost;

	//���ۂɈړ����郁�b�V����̍��W
	Vector3 _position;

	//�ړ��R�X�g
	float _costToMove;

	//�S�[���܂ł̃R�X�g(����)
	float _costToGoal;
};