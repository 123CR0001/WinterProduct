#include"../AppFrame/source/Math/Navi.h"
#include"../AppFrame/source/Math/NavigationMesh.h"
#include<algorithm>

Navi::Navi()
	:_handle(0)
{

}

Navi::~Navi(){}

void Navi::CreateNavigationMesh() {

	std::vector<Polygon3D> polygons;
	GetPolygonData(_handle, polygons);
	NavigationMesh::CreateList(polygons, navMeshes);

}

void Navi::GetPolygonData(int handle, std::vector<Polygon3D>& polygons) {
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
		polygons.emplace_back(myPolygon);
	}
}

bool Navi::LoadModel(const char* fileName) {
	_handle = MV1LoadModel(fileName);

	return _handle > 0 ? true : false;
}

bool Navi::FindPath(const Vector3& startPos, const Vector3& goalPos, std::vector<Vector3>& path, float width) {

	NavigationMesh* start = nullptr;
	NavigationMesh* goal = nullptr;

	Vector3 beginPos;
	Vector3 endPos;

	//開始地点とゴール地点が含まれるNavigationMeshを取得
	for (auto&& navMesh : navMeshes) {

		auto result = HitCheck_Line_Triangle(
			DxConverter::VecToDx(startPos + Vector3(0.f, 1.f, 0.f) * 1000.f),
			DxConverter::VecToDx(startPos + Vector3(0.f, -1.f, 0.f) * 1000.f),
			DxConverter::VecToDx(navMesh.GetMesh().ver1),
			DxConverter::VecToDx(navMesh.GetMesh().ver2),
			DxConverter::VecToDx(navMesh.GetMesh().ver3)
		);

		if( result.HitFlag == 1 ) {
			start = &navMesh;
			beginPos = DxConverter::DxToVec(result.Position);
		}

		result = HitCheck_Line_Triangle(
			DxConverter::VecToDx(goalPos + Vector3(0.f, 1.f, 0.f) * 1000.f),
			DxConverter::VecToDx(goalPos + Vector3(0.f, -1.f, 0.f) * 1000.f),
			DxConverter::VecToDx(navMesh.GetMesh().ver1),
			DxConverter::VecToDx(navMesh.GetMesh().ver2),
			DxConverter::VecToDx(navMesh.GetMesh().ver3)
		);

		if(result.HitFlag == 1) {
			goal = &navMesh;
			endPos = DxConverter::DxToVec(result.Position);
		}

		if(start && goal) {
			break;
		}
	}

	//開始地点とゴール地点が見つからなかった場合
	if(!start || !goal) { return false; }

	auto node = NavigationMesh::FindPath(navMeshes, start, goal, width);

	//経路が見つからなかった場合
	if (!node) { return false; }

	//経路を取得
	while (node != start) {
		path.emplace_back(node->GetPosition());
		node = node->GetParent();
	}

	//NavigationMesh::FindPathで帰ってきたのはゴール地点なので、開始地点を後ろに追加した後で、逆順にする
	path.emplace_back(beginPos);

	std::reverse(path.begin(), path.end());

	path.emplace_back(endPos);

	return true;
}