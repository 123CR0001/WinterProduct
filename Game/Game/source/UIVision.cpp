#include"UIVision.h"
#include"ObjectServer.h"
#include"appframe.h"
#include"CommonSoldier.h"
#include"AIComponent.h"

#include"PhysWorld.h"
#include"FrameComponent.h"

#include"Player.h"

#include"ModeGame.h"
#include"LightsOut.h"

constexpr int SIDE_NUM = 100;

UIVision::UIVision(ObjectServer* server, int drawOrder)
	:UI(drawOrder)
	,_server(server)
	,_handle(LoadGraph("res/UI/Game/visualrange_01.png"))
{
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

	//���C�c�A�E�g���͕`�悵�Ȃ�
	if(!_server->GetGame()->GetLightsOut()->IsUse())return true;

	for(auto&& soldier : _server->GetCommonSoldiers()) {

		//�L�����N�^�[���ނ��Ă���p�x����A
		float rot = soldier->GetEulerAngle().y - DegToRad(soldier->GetAIComponent()->GetViewAngle()) / 2.f;

		//�O�t���[���̒��_�f�[�^���폜
		_vers.clear();

		//���Əd�Ȃ�Ȃ��悤�ɁA������肿����Ə�Ɉʒu�𒲐�
		Vector3 pos = soldier->GetPos() + Vector3(0.f, 1.f, 0.f);

		COLOR_U8 dif = GetColorU8(64,64,64,190);
		COLOR_U8 spc = GetColorU8(64, 64, 64, 190);

		if(soldier->GetAIComponent()->IsFound(_server->GetPlayer())) {
			dif = GetColorU8(128,128,128, 255);
			spc = GetColorU8(128,128,128, 255);
		}

		//���S�_
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

		for(int a = 0; a <= SIDE_NUM; a++) {

			Vector3 viewPos = pos + Vector3(sinf(rot), 0.f, cosf(rot)) * soldier->GetAIComponent()->GetViewDist();

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
				0.5f + 0.5f * (fabsf(viewPos.x - pos.x) / soldier->GetAIComponent()->GetViewDist()),
				0.5f + 0.5f * (fabsf(viewPos.y - pos.y) / soldier->GetAIComponent()->GetViewDist()),
				0.f,
				0.f
			};

			_vers.emplace_back(ver);

			rot += DegToRad(soldier->GetAIComponent()->GetViewAngle()) / SIDE_NUM;

		}


		SetUseLighting(FALSE);
		DrawPolygonIndexed3D(_vers.data(), _vers.size(), _versNums.data(), _versNums.size() / 3, _handle, TRUE);
		SetUseLighting(TRUE);
	}
	return true;
}