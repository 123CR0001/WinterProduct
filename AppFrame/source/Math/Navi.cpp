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
	//���f���Ɋ܂܂����������Ă���
	MV1_REF_POLYGONLIST list = MV1GetReferenceMesh(handle, 0, TRUE);

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

	//�J�n�n�_�ƃS�[���n�_���܂܂��NavigationMesh���擾
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

	//�J�n�n�_�ƃS�[���n�_��������Ȃ������ꍇ
	if(!start || !goal) { return false; }

	auto node = NavigationMesh::FindPath(navMeshes, start, goal, width);

	//�o�H��������Ȃ������ꍇ
	if (!node) { return false; }

	//�o�H���擾
	while (node != start) {
		path.emplace_back(node->GetPosition());
		node = node->GetParent();
	}

	//NavigationMesh::FindPath�ŋA���Ă����̂̓S�[���n�_�Ȃ̂ŁA�J�n�n�_�����ɒǉ�������ŁA�t���ɂ���
	path.emplace_back(beginPos);

	std::reverse(path.begin(), path.end());

	path.emplace_back(endPos);

	return true;
}