#include"../AppFrame/source/Math/NavigationMesh.h"
#include<algorithm>
#include<array>

NavigationMesh::NavigationMesh(const Polygon3D& mesh)
	:_InOpenSet(false)
	, _InClosedSet(false)
	, _Blocked(false)
	, _parent(nullptr)
	, _mesh(mesh)
	, _cost(0.f)
	, _costToMove(0.f)
	, _costToGoal(0.f)
{

}

NavigationMesh::~NavigationMesh()
{
}

NavigationMesh* NavigationMesh::FindPath(std::vector<NavigationMesh>& list, NavigationMesh* start, NavigationMesh* goal, float width) {

	//初期化
	for (auto&& navMesh : list) {
		navMesh.Clear();
	}

	if (!goal || !start) { return nullptr; }

	//最初に調べるナビメッシュ
	NavigationMesh* current = start;
	current->_InClosedSet = true;

	std::vector<NavigationMesh*>openSet;

	do {

		for (auto&& adjacentData : current->_adjacentDataList) {

			//隣接するナビメッシュのアドレス
			auto adjacent = adjacentData.first;

			if (adjacentData.second.segment.Length() < width * 2.f) { continue; }

			//使用できない場合はすぐに次へ
			if (adjacent->_Blocked) { continue; }

			//すでにクローゼットに追加されている場合は次へ
			if (!adjacent->_InClosedSet) {

				if (!adjacent->_InOpenSet) {

					//隣接しているノードの親を自分とする
					adjacent->_parent = current;

					//調べるノード(currentと隣接している)を追加
					openSet.emplace_back(adjacent);

					//すでにオープンセットに追加されていることを知らせ、ここの処理でまた追加されよう
					adjacent->_InOpenSet = true;

					//コストの追加
					adjacent->_position = Vector3::Lerp(adjacentData.second.segment.start, adjacentData.second.segment.end, 0.5f);

					//移動コスト
					adjacent->_costToMove = Vector3::Length(current->_position, adjacent->_position) + current->_costToMove;

					//目標までのコスト(ユークリッド)
					adjacent->_costToGoal = Vector3::Length(adjacent->_position, goal->GetPosition());

					adjacent->_cost = adjacent->_costToMove + adjacent->_costToGoal + current->_costToMove;

				}
				else {

					Vector3 pos = Vector3::Lerp(adjacentData.second.segment.start, adjacentData.second.segment.end, 0.5f);

					float costToMove = Vector3::Length(current->_position, pos) + current->_costToMove;
					float costToGoal = Vector3::Length(adjacent->_position, goal->GetPosition());
					float newCost = current->_costToMove + costToMove + costToGoal;

					//すでに見積もられたコストよりも、新しい見積コストのほうが小さければ、値を更新する
					if (adjacent->_cost > newCost) {
						adjacent->_parent = current;
						adjacent->_cost = newCost;
						adjacent->_position = pos;
						adjacent->_costToGoal = costToGoal;
						adjacent->_costToMove = costToMove;
					}
				}
			}
		}

		if (openSet.empty())
		{
			break;
		}

		//最もコストの低いものを選択
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[](NavigationMesh* a, NavigationMesh* b) {
				return a->_cost < b->_cost;
			});

		//評価しているノードを変更
		current = *iter;

		//オープンセットから削除
		openSet.erase(iter);

		current->_InOpenSet = false;
		current->_InClosedSet = true;

	} while (current != goal);

	return (current == goal) ? current : nullptr;
}

void NavigationMesh::CreateList(const std::vector<Polygon3D>& polys, std::vector<NavigationMesh>& outList) {

	outList.clear();

	for (auto&& poly : polys) {
		outList.emplace_back(NavigationMesh(poly));
	}

	for (int a = 0; a < polys.size(); ++a) {
		for (int b = a + 1; b < polys.size(); ++b) {

			std::vector<Vector3>vers;

			if (Collision::SegPointDistSq(polys[b].ver1, Segment(polys[a].ver1, polys[a].ver2)) < 0.01) {
				vers.emplace_back(polys[b].ver1);
			}
			else if (Collision::SegPointDistSq(polys[b].ver1, Segment(polys[a].ver2, polys[a].ver3)) < 0.01) {
				vers.emplace_back(polys[b].ver1);
			}
			else if (Collision::SegPointDistSq(polys[b].ver1, Segment(polys[a].ver3, polys[a].ver1)) < 0.01) {
				vers.emplace_back(polys[b].ver1);
			}

			//
			if (Collision::SegPointDistSq(polys[b].ver2, Segment(polys[a].ver1, polys[a].ver2)) < 0.01) {
				vers.emplace_back(polys[b].ver2);
			}
			else if (Collision::SegPointDistSq(polys[b].ver2, Segment(polys[a].ver2, polys[a].ver3)) < 0.01) {
				vers.emplace_back(polys[b].ver2);
			}
			else if (Collision::SegPointDistSq(polys[b].ver2, Segment(polys[a].ver3, polys[a].ver1)) < 0.01) {
				vers.emplace_back(polys[b].ver2);
			}

			//
			if (Collision::SegPointDistSq(polys[b].ver3, Segment(polys[a].ver1, polys[a].ver2)) < 0.01) {
				vers.emplace_back(polys[b].ver3);
			}
			else if (Collision::SegPointDistSq(polys[b].ver3, Segment(polys[a].ver2, polys[a].ver3)) < 0.01) {
				vers.emplace_back(polys[b].ver3);
			}
			else if (Collision::SegPointDistSq(polys[b].ver3, Segment(polys[a].ver3, polys[a].ver1)) < 0.01) {
				vers.emplace_back(polys[b].ver3);
			}

			/////////////////////
			if (Collision::SegPointDistSq(polys[a].ver1, Segment(polys[b].ver1, polys[b].ver2)) < 0.01) {
				vers.emplace_back(polys[a].ver1);
			}
			else if (Collision::SegPointDistSq(polys[a].ver1, Segment(polys[b].ver2, polys[b].ver3)) < 0.01) {
				vers.emplace_back(polys[a].ver1);
			}
			else if (Collision::SegPointDistSq(polys[a].ver1, Segment(polys[b].ver3, polys[b].ver1)) < 0.01) {
				vers.emplace_back(polys[a].ver1);
			}

			//
			if (Collision::SegPointDistSq(polys[a].ver2, Segment(polys[b].ver1, polys[b].ver2)) < 0.01) {
				vers.emplace_back(polys[a].ver2);
			}
			else if (Collision::SegPointDistSq(polys[a].ver2, Segment(polys[b].ver2, polys[b].ver3)) < 0.01) {
				vers.emplace_back(polys[a].ver2);
			}
			else if (Collision::SegPointDistSq(polys[a].ver2, Segment(polys[b].ver3, polys[b].ver1)) < 0.01) {
				vers.emplace_back(polys[a].ver2);
			}

			//
			if (Collision::SegPointDistSq(polys[a].ver3, Segment(polys[b].ver1, polys[b].ver2)) < 0.01) {
				vers.emplace_back(polys[a].ver3);
			}
			else if (Collision::SegPointDistSq(polys[a].ver3, Segment(polys[b].ver2, polys[b].ver3)) < 0.01) {
				vers.emplace_back(polys[a].ver3);
			}
			else if (Collision::SegPointDistSq(polys[a].ver3, Segment(polys[b].ver3, polys[b].ver1)) < 0.01) {
				vers.emplace_back(polys[a].ver3);
			}


			//versのsize()が2未満だと、頂点が一つしか重なってしかいないため
			if (vers.size() < 2) { continue; }

			//頂点情報は、順番に並んでいるため、最初と最後の頂点情報は走査だと調べられない。ので、走査前にやっておく
			float longestDistSq = Vector3::LengthSquare(vers.front(), vers.back());

			Vector3 pos = Vector3::Lerp(vers.front(), vers.back(), 0.5f);

			Segment seg(vers.front(), vers.back());

			for (int a = 0; a < vers.size() - 1; a++) {

				float distSq = Vector3::LengthSquare(vers[a], vers[a + 1]);

				if (distSq > longestDistSq) {
					longestDistSq = distSq;
					seg = Segment(vers[a], vers[a + 1]);
					pos = Vector3::Lerp(vers[a], vers[a + 1], 0.5f);
				}

			}

			if (longestDistSq > 0.001f) {
				outList[a].RegisterAdjacent(&outList[b], AdjacentData{ seg });
				outList[b].RegisterAdjacent(&outList[a], AdjacentData{ seg });
			}
		}
	}

}