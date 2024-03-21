#include"ModeClear.h"
#include"ModeTitle.h"

#include "ApplicationMain.h"
#include"ApplicationGlobal.h"

#include "UIServer.h"
#include "UIDisplay.h"
#include "UIClearTime.h"
#include "UIClearTimeRank.h"
#include "UIBlink.h"
#include "UINextStage.h"

#include"ModeColorIn.h"
#include"ModeColorOut.h"
#include"ModeStageSelect.h"

#include"ButtonServer.h"
#include"Button.h"

#include"SpriteText.h"
#include"SpriteNumber.h"
#include"TransformAnimation.h"
#include"OpacityAnimation.h"
#include"EasingNumber.h"

#include"MyUIServer.h"
#include"TimeLine.h"
#include"UIHrsMinSec.h"
#include"UISpriteText.h"

ModeClear::ModeClear(std::shared_ptr<ModeGame::ResultData> data)
	:_resultData(data)
	,_uiServer(NEW MyUIServer())
	,_buttonServer(NEW ButtonServer())
	,_timeLine(NEW TimeLine())
{
	
	SetUI();
	SetButton();

}

bool ModeClear::Initialize() {
	gGlobal._sndServer.Play("BGM_08");
	return true;
}

bool ModeClear::Terminate() {
	delete _buttonServer;
	delete _uiServer;
	delete _timeLine;
	return true;
}

bool ModeClear::Process() {
	base::Process();

	_uiServer->Process();
	_buttonServer->Process();
	_timeLine->Process();
	return true;
}

bool ModeClear::Render() {
	base::Render();

	_uiServer->Draw();
	_buttonServer->Draw();

	return true;
}

void ModeClear::SetUI() {


	//データ枠
	{
		auto text = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_scorebg_01.png"),
			Transform2(Vector2(3000.f * SCREEN_WIDTH_MAG, 244.f * SCREEN_HEIGHT_MAG)),
			Vector2(896.f * SCREEN_WIDTH_MAG, 366.f * SCREEN_HEIGHT_MAG)
		);
		text->AddAnimation(NEW TransformAnimation(text, 60.f, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 244.f * SCREEN_HEIGHT_MAG))));
		_uiServer->AddUI(NEW UISpriteText(text));
	}

	//ボタン外枠
	{
		auto text = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_scorebg_02.png"),
			Transform2(Vector2(3000.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG)),
			Vector2(896.f * SCREEN_WIDTH_MAG, 232.f * SCREEN_HEIGHT_MAG)
		);
		text->AddAnimation(NEW TransformAnimation(text, 60.f, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG))));
		_uiServer->AddUI(NEW UISpriteText(text));
	}

	//クリアタイム
	{
		auto func = [=]()mutable {
			_uiServer->AddUI(
				NEW UIHrsMinSec(30, _resultData->clearSecondTime,Transform2(Vector2(1766.f * SCREEN_WIDTH_MAG,192.5f * SCREEN_HEIGHT_MAG)),Vector2(46.f,70.f))
			);
		};
		//MyUIServerのProcess()が70回呼ばれたら、処理する
		_timeLine->AddLine(70, func);
	}

	//最高検知度
	{
		auto func = [=]() mutable{
			SpriteNumber* number = NEW SpriteNumber(static_cast<int>(_resultData->maxDetectionLevel * 100.f));
			number->AddAnimation(NEW EasingNumber(number, 30)); 
			number->LoadDivNumber("res/UI/Result/ui_timer_01.png",5,2,46,70);
			number->SetSize(Vector2(46.f * SCREEN_WIDTH_MAG, 70.f * SCREEN_HEIGHT_MAG));
			number->SetPos(Vector2(1709.f * SCREEN_WIDTH_MAG, 282.5f * SCREEN_HEIGHT_MAG));
			_uiServer->AddUI(NEW UISpriteText(number));
		};

		_timeLine->AddLine(120, func);
	}

	//最高キルコンボ
	{
		auto func = [=]() mutable {
			SpriteNumber* number = NEW SpriteNumber(_resultData->maxCombo);
			number->AddAnimation(NEW EasingNumber(number, 30));
			number->LoadDivNumber("res/UI/Result/ui_timer_01.png", 5, 2, 46, 70);
			number->SetSize(Vector2(46.f * SCREEN_WIDTH_MAG, 70.f * SCREEN_HEIGHT_MAG));
			number->SetPos(Vector2(1709.f * SCREEN_WIDTH_MAG, 372.5f * SCREEN_HEIGHT_MAG));
			_uiServer->AddUI(NEW UISpriteText(number));
		};

		_timeLine->AddLine(180, func);
	}

	//エンブレム
	{
		auto func = [=]() mutable {
			std::string fileName = "res/UI/Result/Emblem/ui_rankemblem_";
			fileName += _resultData->GetRank();
			fileName += ".png";
			SpriteText* text = NEW SpriteText(
				ResourceServer::LoadGraph(fileName.c_str()),
					//			回転	拡大	座標
					Transform2(0.f,		3.f,	Vector2(1344.f * SCREEN_WIDTH_MAG, 589.5f * SCREEN_HEIGHT_MAG)),
					Vector2(640.f * SCREEN_WIDTH_MAG, 230.f * SCREEN_HEIGHT_MAG),
					0.f
			);
			text->AddAnimation(NEW TransformAnimation(text,30,Transform2(0.f, 1.f, Vector2(SCREEN_WIDTH_MAG * 1344.f, SCREEN_HEIGHT_MAG * 589.5f))));
			text->AddAnimation(NEW OpacityAnimation(text, 30, 1.f));
			_uiServer->AddUI(NEW UISpriteText(text));
		};

		_timeLine->AddLine(240, func);
	}

	{
		SpriteText* text = NEW SpriteText();
		text->SetHandle(ResourceServer::LoadGraph("res/UI/Result/ui_resultbg_01.png"));
		text->SetSize(Vector2(static_cast<float>(SCREEN_WIDTH),static_cast<float>(SCREEN_HEIGHT)));
		text->SetPos(Vector2(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2)));

		_uiServer->AddUI(NEW UISpriteText(text, 1000));
	}
}

void ModeClear::SetButton() {


	{
		auto targetUI = _buttonServer->GetSelectUI();

		targetUI->SetHandle(ResourceServer::LoadGraph("res/UI/Result/ui_target_01.png"));
		targetUI->SetSize(Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG));
		targetUI->SetAlpha(1.f);
		targetUI->AddAnimation(NEW OpacityAnimation(targetUI, -120, 0.6f));
	}

	//ステージセレクト
	{
		SpriteText* button = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_stageselection_01.png"),
			Transform2(Vector2(3000.f * SCREEN_WIDTH_MAG, 824.f * SCREEN_HEIGHT_MAG)),
			Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG)
		);
		button->AddAnimation(NEW TransformAnimation(button, 60.f, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 824.f * SCREEN_HEIGHT_MAG))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// モードの削除
						ModeServer::GetInstance()->Del(this);
						// 次のモードを登録
						ModeServer::GetInstance()->Add(NEW ModeStageSelect(), 100, "select");
						};
					// 次のモードを登録
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");

					_buttonServer->SetStep(ButtonServer::STEP::kConclude);
				},
				button
			)
		);

	}
	{
		//画像の設定
		SpriteText* titleButton = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_playagain_01.png"),
			Transform2(Vector2(Vector2(3000.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG))),
			Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG)
		);
		//アニメーションの設定
		titleButton->AddAnimation(NEW TransformAnimation(titleButton, 60.f, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 896.f * SCREEN_HEIGHT_MAG))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// モードの削除
						ModeServer::GetInstance()->Del(this);
						// 次のモードを登録
						ModeServer::GetInstance()->Add(NEW ModeGame(_resultData->_stageName), 1, "game");
					};
					// 次のモードを登録
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");

					_buttonServer->SetStep(ButtonServer::STEP::kConclude);
				},
				titleButton
					)
		);
	}
	if (_resultData->_nextStageName.size() > 0)
	{
		//画像の設定
		SpriteText* titleButton = NEW SpriteText(
			ResourceServer::LoadGraph("res/UI/Result/ui_nextstage_01.png"),
			Transform2(Vector2(Vector2(3000.f * SCREEN_WIDTH_MAG, 970.f * SCREEN_HEIGHT_MAG))),
			Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG)
		);
		//アニメーションの設定
		titleButton->AddAnimation(NEW TransformAnimation(titleButton, 60.f, Transform2(Vector2(1344.f * SCREEN_WIDTH_MAG, 970.f* SCREEN_HEIGHT_MAG))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// モードの削除
						ModeServer::GetInstance()->Del(this);
						// 次のモードを登録
						ModeServer::GetInstance()->Add(NEW ModeGame(_resultData->_nextStageName), 1, "game");
					};
					// 次のモードを登録
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");

					_buttonServer->SetStep(ButtonServer::STEP::kConclude);
				},
				titleButton
					)
		);
	}
	
}

