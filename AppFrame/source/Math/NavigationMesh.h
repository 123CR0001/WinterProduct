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

		//すでに登録されている場合は登録しない
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
	//隣接するポリゴン
	std::vector<std::pair<NavigationMesh*, AdjacentData>>_adjacentDataList;

	//親
	NavigationMesh* _parent;

	//メッシュ
	Polygon3D _mesh;

	//オープンセット
	bool _InOpenSet;
	//クローゼット
	bool _InClosedSet;

	//ナビメッシュ(道)として使用できるか
	bool _Blocked;

	//コスト
	float _cost;

	//実際に移動するメッシュ状の座標
	Vector3 _position;

	//移動コスト
	float _costToMove;

	//ゴールまでのコスト(見積)
	float _costToGoal;
};