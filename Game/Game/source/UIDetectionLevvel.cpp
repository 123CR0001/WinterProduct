#include"UIDetectionLevel.h"
#include"SpriteText.h"
#include"ObjectServer.h"
#include"AIComponent.h"
#include"CommonSoldier.h"
#include"Player.h"

#include"LightsOut.h"
#include"ModeGame.h"

UIDetectionLevel::UIDetectionLevel(ObjectServer* server,int drawOrder)
	:UI(drawOrder)
	,_gaugeHandle(ResourceServer::LoadGraph("res/UI/Game/cir.png"))
	,_gaugeBgHandle(ResourceServer::LoadGraph("res/UI/Game/black.png"))
	,_server(server)
{

}

UIDetectionLevel::~UIDetectionLevel(){

}

bool UIDetectionLevel::Process() {

	return true;
}

bool UIDetectionLevel::Draw() {

	if(!_server->GetGame()->GetLightsOut()->IsUse())return true;

	auto player = _server->GetPlayer();

	for(auto iter = _server->GetCommonSoldiers().begin(); iter != _server->GetCommonSoldiers().end(); ++iter) {
		
		float alpha = 0.5f;

		if((*iter)->GetAIComponent()->IsFound(player)) {
			alpha = 1.f;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * alpha));

		VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx((*iter)->GetPos() + Vector3(50.f, 170.f, 0.f)));

		double level = static_cast<double>((*iter)->GetDetectionLevel() * 100.f);

		DrawCircleGaugeF(pos.x, pos.y, 100, _gaugeBgHandle);
		DrawCircleGaugeF(pos.x, pos.y, level, _gaugeHandle);

		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	return true;
}