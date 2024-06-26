#include"UIVision.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"AIComponent.h"

#include"PhysWorld.h"
#include"FrameComponent.h"

#include"Player.h"

#include"ModeGame.h"

constexpr int SIDE_NUM = 100;

UIVision::UIVision(ObjectServer* server, int drawOrder)
	:UI(drawOrder)
	,_server(server)
	,_handle(LoadGraph("res/UI/Game/visualrange_01.png"))
{
	//描画する扇のポリゴンインデックスを辺(円周)の分だけ、生成する
	for(int a = 0; a < SIDE_NUM; a++) {
		_versNums.emplace_back(0);
		_versNums.emplace_back(a + 1);
		_versNums.emplace_back(a + 2);
	}
}

UIVision::~UIVision(){}

bool UIVision::Process() {
	return true;
}

bool UIVision::Draw() {

	//ライツアウト中は描画しない
	if(_server->GetGame()->IsUsingLightsOut())return true;

	for(auto&& soldier : _server->GetCommonSoldiers()) {

		//キャラクターがむいている角度から、
		float rot = soldier->GetEulerAngle().y - DegToRad(soldier->GetAIComponent()->GetViewAngle()) / 2.f;

		//前フレームの頂点データを削除
		_vers.clear();

		//床と重ならないように、足元よりちょっと上に位置を調整
		Vector3 pos = soldier->GetPos() + Vector3(0.f, 1.f, 0.f);

		COLOR_U8 dif = GetColorU8(64,64,64,190);
		COLOR_U8 spc = GetColorU8(64, 64, 64, 190);

		if(soldier->GetAIComponent()->IsFound(_server->GetPlayer())) {
			dif = GetColorU8(128,128,128, 255);
			spc = GetColorU8(128,128,128, 255);
		}

		//中心点
		_vers.emplace_back(
			VERTEX3D{
				DxConverter::VecToDx(Vector3(pos)),
				VGet(0.f,1.f,0.f),
				dif,
				spc,
				0.5f,
				0.5f,
				0.f,
				0.f
			}
		);

		//扇の辺(円周)の頂点を生成
		for(int a = 0; a <= SIDE_NUM; a++) {

			Vector3 viewPos = pos + Vector3(sinf(rot), 0.f, cosf(rot)) * soldier->GetAIComponent()->GetViewDist();

			//キャラクターの位置から、上記の座標までにオブジェクト(押し出し処理のする)に遮られているか
			for(auto&& frame : _server->GetPhysWorld()->GetFrameComponent()) {
				MV1_COLL_RESULT_POLY result =
					MV1CollCheck_Line(
						frame->GetOwner()->GetHandle(),
						frame->GetOwner()->GetAttachIndex(),
						DxConverter::VecToDx(pos),
						DxConverter::VecToDx(viewPos)
					);
				//遮られていたら、頂点の座標をぶつかった座標にする
				if(result.HitFlag) { viewPos = DxConverter::DxToVec(result.HitPosition); }
			}

			//頂点
			VERTEX3D ver = {
				DxConverter::VecToDx(viewPos),
				VGet(0.f,1.f,0.f),
				dif,
				spc,
				0.5f + 0.5f * (fabsf(viewPos.x - pos.x) / soldier->GetAIComponent()->GetViewDist()),
				0.5f + 0.5f * (fabsf(viewPos.y - pos.y) / soldier->GetAIComponent()->GetViewDist()),
				0.f,
				0.f
			};

			//追加
			_vers.emplace_back(ver);

			//角度
			rot += DegToRad(soldier->GetAIComponent()->GetViewAngle()) / SIDE_NUM;

		}

		//UIなので、ライティングを無効にして描画
		SetUseLighting(FALSE);
		DrawPolygonIndexed3D(_vers.data(), static_cast<int>(_vers.size()), _versNums.data(), static_cast<float>(_versNums.size() / 3), _handle, TRUE);
		SetUseLighting(TRUE);
	}
	return true;
}