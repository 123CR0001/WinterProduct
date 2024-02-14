#include"Container.h"
#include"ModeGame.h"
#include"ObjectServer.h"
#include"Player.h"

#include<algorithm>

Container::Container(ObjectServer* server) 
	:ObjectBase(server)
{
	server->GetFrames().emplace_back(this);
}

Container::~Container(){

}

bool Container::Initialize() {
	//_handle = MV1LoadModel("res/SDChar/SDChar.mv1");
	// 3Dモデルの1番目のアニメーションをアタッチする
	//SetUseTransColor(1); 
	//SetUsePremulAlphaConvertLoad(1);

	LoadModel("res/Object/container.mv1");
	_attachIndex =  MV1SearchFrame(_handle, "cube");
	//MV1SetRotationXYZ(_handle,VGet(0,0,-1));
	MV1SetupCollInfo(_handle, _attachIndex, 8, 8, 8);

	_is_fall = 0;

	//GetServer()->AddObj(this);


	_count = 30;
	ObjectBase::Initialize();
	return true;
}


bool Container::Terminate(){
	auto iter = std::find(GetObjectServer()->GetFrames().begin(),
		GetObjectServer()->GetFrames().end(),
		this
	);

	GetObjectServer()->GetFrames().erase(iter);
	return true;
}

bool Container::Process(){
	_oldPos = _pos;

	//_euler_angle.z += 1 * 3.14 / 180;


	MV1RefreshCollInfo(_handle, _attachIndex);

	if (GetObjectServer()->GetPlayer()->GetOnObject() == this) {
		GetObjectServer()->GetPlayer()->AddPos(_pos - _oldPos);
		_is_fall = true;
	}

	if (_is_fall) {
		_count--;
		if (_count < 0 ) {
			_pos.y -= 30;
		}
		if (_count < -120) {
			_is_fall = false;
		}
		
	}
	
	float rad = GetObjectServer()->GetPlayer()->GetRadian();
	float colsub = GetObjectServer()->GetPlayer()->GetCollSubY();
	auto pos = GetObjectServer()->GetPlayer()->GetPos();

	MV1_COLL_RESULT_POLY_DIM hitXZ = MV1CollCheck_Capsule(
		_handle,
		_attachIndex,
		DxConverter::VecToDx(pos + Vector3D(0, rad, 0)),
		DxConverter::VecToDx(pos + Vector3D(0, rad + colsub, 0)),
		rad
	);
	if (hitXZ.Dim) {

		double dist = 9999999999;

		Vector3D under_pos(pos);
		under_pos.y += rad;
		Vector3D up_pos(pos);
		up_pos.y += (colsub + rad);

		Segment3D coll_seg(under_pos, up_pos);
		Vector3D pol_latest_point(0, 0, 0);
		Vector3D add_base_point(0, 0, 0);

		for (int a = 0; a < hitXZ.HitNum; a++) {
			Polygon3D detection = Polygon3D(
				DxConverter::DxToVec(hitXZ.Dim[a].Position[0]),
				DxConverter::DxToVec(hitXZ.Dim[a].Position[1]),
				DxConverter::DxToVec(hitXZ.Dim[a].Position[2])
			);

			SEGMENT_TRIANGLE_RESULT result = Collision::SegmentPolygonAnalyze(detection, coll_seg);

			double dist_com = result.Seg_Tri_MinDist_Square;

			if (dist_com < dist) {
				dist = dist_com;
				pol_latest_point = DxConverter::DxToVec(result.Tri_MinDist_Pos);
				add_base_point = DxConverter::DxToVec(result.Seg_MinDist_Pos);
			}
		}

		if (dist < rad * rad) {
			Vector3D latest_v(pol_latest_point - add_base_point);
			latest_v.Normalized();
			latest_v *= (rad);
			Vector3D add((pol_latest_point - latest_v));
			GetObjectServer()->GetPlayer()->AddPos(add - add_base_point);
		}

	}
	MV1CollResultPolyDimTerminate(hitXZ);

	ObjectBase::Process();
	return true;
}

bool Container::Render() {
	ObjectBase::Render();

	return true;
}