
#include "ApplicationMain.h"
#include "UIDetection.h"
#include "ModeGame.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"

UIDetection::UIDetection(class ModeGame* game):_game(game) {
	// 初期化
	_eneInfo.bFind = false;
	_eneInfo.val = 100;

	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();
	for (auto iter : CommonSoldiers) {
		_vEneInfo.push_back(_eneInfo);
	}
}

void UIDetection::Process() {
	// 敵に見つかったら_bFindをtrueにする
	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();
	int i = 0;
	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {
		if ((*iter)->IsPlayerFound()) { 
			_vEneInfo[i].bFind = true;
		}
		else if(!(*iter)->IsPlayerFound()){
			_vEneInfo[i].bFind = false;
		}
		i++;
	}
	for (int j = 0; j < _vEneInfo.size(); j++) {
		// 見つかっている間はゲージが上昇する
		if (_vEneInfo[j].bFind) { _vEneInfo[j].val--; }
		// 視界から外れたら、ゲージが減少する
		else {
			if (_vEneInfo[j].val < 100) { _vEneInfo[j].val++; }
		}
		_vEneInfo[j].val = Clamp(0, 100, _vEneInfo[j].val);
	}
	
}

void UIDetection::Render() {
	// 画像読み込み
	int cgBaseCircle = ResourceServer::LoadGraph("res/UI/Game/cir.png");
	int cgQuestion = ResourceServer::LoadGraph("res/UI/Game/question.png");
	int cgSuprise = ResourceServer::LoadGraph("res/UI/Game/surprise.png");

	// playerを発見している敵を見つける
	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();
	int i = 0;
	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {
		if ((*iter)->IsPlayerFound() || _vEneInfo[i].val < 100) {										// 敵に発見されている、或いは検知度が残っていたら描画する
			VECTOR vPos = (*iter)->GetDxPos();													// 敵の位置座標を取り出す
			VECTOR highVec = { 0,230,0 };														// 
			VECTOR eneOverhead = VAdd(vPos, highVec);											// 敵の頭上に表示するためにベクトルを加算する
			VECTOR convertScreenPos = ConvWorldPosToScreenPos(eneOverhead);						// ワールド座標をスクリーン座標に変換する
																								// 
			DrawGraph(convertScreenPos.x - 50, convertScreenPos.y - 50, cgBaseCircle, TRUE);	// ベース用の色付きサークル
			DrawCircleGauge(convertScreenPos.x, convertScreenPos.y, _vEneInfo[i].val, _cg);					// 実際にゲージの上昇・減少を描画するサークル　黒
			if (_vEneInfo[i].val == 0) {
				DrawGraph(convertScreenPos.x - 40, convertScreenPos.y - 40, cgSuprise, TRUE);	// 「！」見つかった
			}
			else {
				DrawGraph(convertScreenPos.x - 40, convertScreenPos.y - 40, cgQuestion, TRUE);	// 「？」疑問
			}
		}
		i++;
	}
}
