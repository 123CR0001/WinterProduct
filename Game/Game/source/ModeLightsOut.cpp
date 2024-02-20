#include"ModeLightsOut.h"
#include"ModeColorOut.h"
#include"ModeColorIn.h"
#include"ModeGame.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"Player.h"

ModeLightsOut::ModeLightsOut(ModeGame* game)
	:_game(game) 
	,_oldEnemyNum((int)game->GetObjectServer()->GetEnemys().size())
{

}

bool ModeLightsOut::Initialize() {

	std::vector<int>noiseCgs;
	noiseCgs.resize(6);
	ResourceServer::LoadDivGraph("res/Effect/ui_nightscope_1.png",
		6,
		1,
		6, 
		1920,
		1080,
		noiseCgs.data()
	);
	_noiseAnim = new Animation2D(
		noiseCgs,
		10,
		0, 0, ApplicationBase::GetInstance()->DispSizeW(), ApplicationBase::GetInstance()->DispSizeH()
	);

	_cg = ResourceServer::LoadGraph("res/Effect/nightscope_01.png");

	_alpha = 170;

	_frameCnt = 300;

	return true;
}

bool ModeLightsOut::Terminate() {

	return true;
}

bool ModeLightsOut::Process() {

	if (_frameCnt <= 0 && !ModeServer::GetInstance()->IsAdd("Out")) {

		auto func = [this]() {
			ModeServer::GetInstance()->Del(this);
		};

		//フェードアウト
		ModeColorIn* colorIn = new ModeColorIn(10);
		ModeServer::GetInstance()->Add(new ModeColorOut(colorIn,func, 10), 10, "Out");
	}
	if(_frameCnt > 0) {
		_frameCnt--;
	}

	if (_oldEnemyNum > (int)_game->GetObjectServer()->GetEnemys().size()) {
		_frameCnt += 60;
		_oldEnemyNum = (int)_game->GetObjectServer()->GetEnemys().size();
		_game->GetObjectServer()->GetPlayer()->AddMoveSpeedMag(0.2f);
	}

	return true;
}

bool ModeLightsOut::Render() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);	// 半透明モード
	auto instance = ApplicationBase::GetInstance();
	DrawModiGraph(
		0, 0,
		instance->DispSizeW(),0,
		instance->DispSizeW(), instance->DispSizeH(),
		0, instance->DispSizeH(),
		_cg,
		TRUE
	);
	_noiseAnim->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alpha);

	SetFontSize(64);
	DrawFormatString(500, 0, GetColor(255, 0, 0), "残り時間 %d", _frameCnt);
	SetFontSize(16);

	return true;
}