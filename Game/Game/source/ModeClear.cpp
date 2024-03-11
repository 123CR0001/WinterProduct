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
#include"ModeSelect.h"

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
	
	DrawFormatString(0, 0, GetColor(0, 255, 0), "select Num %d", _buttonServer->GetSelectNum());

	return true;
}

void ModeClear::SetUI() {
	int screenWidth = ApplicationMain::GetInstance()->DispSizeW();
	int screenHeight = ApplicationMain::GetInstance()->DispSizeH();

	const float rateW = static_cast<float>(screenWidth) / static_cast<float>(1920);
	const float rateH = static_cast<float>(screenHeight) / static_cast<float>(1080);

	//リゼルとデータ枠
	{
		auto text = NEW SpriteText(
			LoadGraph("res/UI/Result/ui_scorebg_01.png"),
			Transform2(Vector2(screenWidth * 1.5f, screenHeight * 0.25f)),
			Vector2(896.f * rateW, 232.f * rateH)
		);
		text->AddAnimation(NEW TransformAnimation(text, 60.f, Transform2(Vector2(screenWidth * 0.75f,screenHeight * 0.25f))));
		_uiServer->AddUI(NEW UISpriteText(text));
	}

	//ボタン外枠
	{
		auto text = NEW SpriteText(
			LoadGraph("res/UI/Result/ui_scorebg_02.png"),
			Transform2(Vector2(screenWidth * 1.5f, screenHeight * 0.75f)),
			Vector2(896.f * rateW, 232.f * rateH)
		);
		text->AddAnimation(NEW TransformAnimation(text, 60.f, Transform2(Vector2(screenWidth * 0.75f, screenHeight * 0.75f))));
		_uiServer->AddUI(NEW UISpriteText(text));
	}

	//クリアタイム
	{
		auto func = [this]() {_uiServer->AddUI(NEW UIHrsMinSec(30, _resultData->clearSecondTime)); };
		//MyUIServerのProcess()が70回呼ばれたら、処理する
		_timeLine->AddLine(70, func);
	}

	//最高検知度
	{
		auto func = [=]() mutable{
			SpriteNumber* number = NEW SpriteNumber(static_cast<int>(_resultData->maxDetectionLevel * 100.f));
			number->AddAnimation(NEW EasingNumber(number, 30)); 
			number->LoadDivNumber("res/UI/Result/ui_timer_01.png",5,2,46,70);
			number->SetSize(Vector2(23.f * rateW, 35.f * rateH));
			number->SetPos(Vector2(screenWidth * 0.92f, screenHeight*0.28f));
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
			number->SetSize(Vector2(23.f * rateW, 35.f * rateH));
			number->SetPos(Vector2(screenWidth * 0.92f, screenHeight * 0.33f));
			_uiServer->AddUI(NEW UISpriteText(number));
		};

		_timeLine->AddLine(180, func);
	}

	//エンブレム
	{
		auto func = [=]() mutable {
			SpriteText* text = NEW SpriteText(
					LoadGraph("res/UI/Result/Emblem/ui_renkemblem_s.png"), 
					//			回転	拡大	座標
					Transform2(0.f,		3.f,	Vector2(1024.f * rateW, 448.f * rateH)),
					Vector2(640.f * rateW, 230.f * rateH),
					0.f
			);
			text->AddAnimation(NEW TransformAnimation(text,30,Transform2(0.f, 1.f, Vector2(screenWidth * 0.8f, screenHeight * 0.5f))));
			text->AddAnimation(NEW OpacityAnimation(text, 30, 1.f));
			_uiServer->AddUI(NEW UISpriteText(text));
		};

		_timeLine->AddLine(240, func);
	}
}

void ModeClear::SetButton() {
	int screenWidth = ApplicationMain::GetInstance()->DispSizeW();
	int screenHeight = ApplicationMain::GetInstance()->DispSizeH();

	const float rateW = static_cast<float>(screenWidth) / static_cast<float>(1920);
	const float rateH = static_cast<float>(screenHeight) / static_cast<float>(1080);

	{
		auto targetUI = _buttonServer->GetSelectUI();

		targetUI->SetHandle(LoadGraph("res/UI/Result/ui_target_01.png"));
		targetUI->SetSize(Vector2(384.f * rateW, 64.f * rateH));
		targetUI->SetAlpha(1.f);
		targetUI->AddAnimation(NEW OpacityAnimation(targetUI, -120, 0.6f));
	}
	{
		//画像の設定
		SpriteText* titleButton = NEW SpriteText(
			LoadGraph("res/UI/Result/ui_stageselection_01.png"),
			Transform2(Vector2(screenWidth * 1.5f, screenHeight * 0.72f)),
			Vector2(384.f * rateW, 64.f * rateH)
		);
		//アニメーションの設定
		titleButton->AddAnimation(NEW TransformAnimation(titleButton, 60.f, Transform2(Vector2(screenWidth * 0.75f, screenHeight * 0.68f))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// モードの削除
						ModeServer::GetInstance()->Del(this);
						// 次のモードを登録
						ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "title");
					};
					// 次のモードを登録
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");
				},
				titleButton
					)
		);
	}
	{
		SpriteText* button = NEW SpriteText(
			LoadGraph("res/UI/Result/ui_nextstage_01.png"),
			Transform2(Vector2(screenWidth * 1.5f, screenHeight * 0.8f)),
			Vector2(384.f * rateW, 64.f * rateH)
		);
		button->AddAnimation(NEW TransformAnimation(button, 60.f, Transform2(Vector2(screenWidth * 0.75f, screenHeight * 0.75f))));

		_buttonServer->AddButton(
			NEW Button(
				_buttonServer,

				[this]() {
					auto func = [this]() {
						// モードの削除
						ModeServer::GetInstance()->Del(this);
						// 次のモードを登録
						ModeServer::GetInstance()->Add(NEW ModeSelect(), 100, "select");
					};
					// 次のモードを登録
					ModeBase* mode = NEW ModeColorOut(NEW ModeColorIn(60, true), func, 60);
					ModeServer::GetInstance()->Add(mode, 100, "Out");
				},
				button
					)
		);

	}
}