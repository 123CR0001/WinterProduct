#include"UIVision.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"AIComponent.h"

#include"PhysWorld.h"
#include"FrameComponent.h"

#include"Player.h"

constexpr int SIDE_NUM = 100;

UIVision::UIVision(ObjectServer* server)
	:_server(server)
	,_handle(LoadGraph("res/UI/visualrange_01.png"))
{

}

UIVision::~UIVision(){}

bool UIVision::Process() {


	return true;
}

bool UIVision::Draw() {
	for(auto&& soldier : _server->GetCommonSoldiers()) {

		//キャラクターがむいている角度から、
		float rot = soldier->GetEulerAngle().y - DegToRad(soldier->GetAIComponent()->GetViewAngle()) / 2.f;

		//前フレームの頂点データを削除
		_vers.clear();

		//床と重ならないように、足元よりちょっと上に位置を調整
		Vector3D pos = soldier->GetPos() + Vector3D(0.f, 1.f, 0.f);

		int handle = _handle;
		COLOR_U8 dif = GetColorU8(255, 0, 0, 0);
		COLOR_U8 spc = GetColorU8(255, 0, 0, 0);

		if(soldier->GetAIComponent()->IsFound(_server->GetPlayer())) {
			dif = GetColorU8(0, 0, 255, 0);
			spc = GetColorU8(0, 0, 255, 0);
		}

		//中心点
		_vers.emplace_back(
			VERTEX3D{
				DxConverter::VecToDx(Vector3D(pos)),
				VGet(0.f,1.f,0.f),
				dif,
				spc,
				0.5f,
				0.5f,
				0.f,
				0.f
			}
		);

		for(int a = 0; a <= SIDE_NUM; a++) {

			Vector3D viewPos = pos + Vector3D(sinf(rot), 0.f, cosf(rot)) * soldier->GetAIComponent()->GetViewDist();

			for(auto&& frame : _server->GetPhysWorld()->GetFrameComponent()) {
				MV1_COLL_RESULT_POLY result =
					MV1CollCheck_Line(
						frame->GetOwner()->GetHandle(),
						frame->GetOwner()->GetAttachIndex(),
						DxConverter::VecToDx(pos),
						DxConverter::VecToDx(viewPos)
					);

				if(result.HitFlag) { viewPos = DxConverter::DxToVec(result.HitPosition); }
			}

			VERTEX3D ver = {
				DxConverter::VecToDx(viewPos),
				VGet(0.f,1.f,0.f),
				dif,
				spc,
				0.5f + 0.5f * (Vector3D::Length(viewPos,pos) / soldier->GetAIComponent()->GetViewDist()),
				0.5f + 0.5f * (Vector3D::Length(viewPos,pos) / soldier->GetAIComponent()->GetViewDist()),
				0.f,
				0.f
			};

			_vers.emplace_back(ver);

			rot += DegToRad(soldier->GetAIComponent()->GetViewAngle()) / SIDE_NUM;

		}


		SetUseLighting(FALSE);
		DrawPolygonIndexed3D(_vers.data(), _vers.size(), _versNums.data(), _versNums.size() / 3, handle, FALSE);
		SetUseLighting(TRUE);
	}
	return true;
}