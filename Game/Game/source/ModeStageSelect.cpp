#include"ModeStageSelect.h"
#include"SpriteText.h"
#include"ButtonServer.h"
#include"Button.h"
#include"TransformAnimation.h"
#include"ApplicationMain.h"

bool ModeStageSelect::Initialize() {


	_buttonServer = NEW ButtonServer();

	{
		auto targetUI = _buttonServer->GetSelectUI();

		targetUI->SetHandle(ResourceServer::LoadGraph("res/UI/Result/ui_target_01.png"));
		targetUI->SetSize(Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG));
		targetUI->SetAlpha(1.f);
	}

	//stage1_
	for (int num = 0; num < 3; num++) {
		std::string filePath = "res/UI/StageSelect/stage";
		filePath+= std::to_string(num + 1);
		filePath +="/ui_stage0";
		filePath += std::to_string(num + 1);
		std::string stageType = filePath + ".png";

		auto text = NEW SpriteText();
		text->SetHandle(ResourceServer::LoadGraph(stageType.c_str()));
		text->SetSize(Vector2(600.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG));
		text->SetPos(Vector2(-1000.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG + 200.f * SCREEN_HEIGHT_MAG * num));
		text->AddAnimation(NEW TransformAnimation(text, 30, Transform2(Vector2(300.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG + 200.f * SCREEN_HEIGHT_MAG * num))));
		
		auto selectFunc = [=]()mutable {

			//アニメーションの逆再生
			for (int a = 0; a < _buttonServer->GetButtons().size(); a++) {
				if (a == _buttonServer->GetSelectNum()) { 
					continue;
				}
				_buttonServer->GetButtons()[a]->GetSpriteText()->GetAnimations().back()->Reverse();

				auto animEndFunc = [=]()mutable {_buttonServer->DeleteButton(_buttonServer->GetButtons()[a]); };
				_buttonServer->GetButtons()[a]->GetSpriteText()->GetAnimations().back()->SetFunc(animEndFunc);
			}

			for (int a = 0; a < 3; a++) {
				//stage1_
				{
					auto _text = NEW SpriteText();
					std::string name = filePath + "_";
					name += std::to_string(a + 1);
					name += ".png";

					_text->SetHandle(ResourceServer::LoadGraph(name.c_str()));
					_text->SetSize(Vector2(600.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG));

					float diff_y = 200.f * SCREEN_HEIGHT_MAG * a;

					//																			少しずつずらす
					_text->SetPos(Vector2(-1000.f * SCREEN_WIDTH_MAG, 300.f * SCREEN_HEIGHT_MAG + diff_y));
					_text->AddAnimation(NEW TransformAnimation(_text, 30, Transform2(Vector2(300.f * SCREEN_WIDTH_MAG, 300.f * SCREEN_HEIGHT_MAG + diff_y))));

					auto selectFunc = [=]()mutable {
						
					};

					_buttonServer->AddButton(NEW Button(_buttonServer, selectFunc, text));
				}
			}

			//上に移動
			text->AddAnimation(NEW TransformAnimation(text, 10, Transform2(Vector2(300.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG))));

			_buttonServer->SetSelectNum(0);

		};

		_buttonServer->AddButton(NEW Button(_buttonServer, selectFunc, text));
	}


	return true;
}

bool ModeStageSelect::Terminate() {
	delete _buttonServer;
	return true;
}

bool ModeStageSelect::Process() {
	_buttonServer->Process();
	return true;
}

bool ModeStageSelect::Render() {
	_buttonServer->Draw();

	int dy = 0;

	DrawFormatString(0, dy, GetColor(255, 0, 0), "ボタンの数:%d", _buttonServer->GetButtons().size()); dy += 25;

	switch (_buttonServer->GetStep()) {
	case ButtonServer::STEP::kAnimation:
		DrawFormatString(0, dy, GetColor(255, 0, 0), "アニメーション"); dy += 25;
		break;
	case ButtonServer::STEP::kProcess:
		DrawFormatString(0, dy, GetColor(255, 0, 0), "ボタンの処理"); dy += 25;
		break;
	}
	return true;
}

