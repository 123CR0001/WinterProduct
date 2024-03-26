#include"ModeStageSelect.h"
#include"SpriteText.h"
#include"ButtonServer.h"
#include"Button.h"
#include"TransformAnimation.h"
#include"ApplicationMain.h"
#include"ApplicationGlobal.h"
#include"ModeGame.h"
#include"ModeTitle.h"
#include"ModeColorIn.h"
#include"ModeColorOut.h"
constexpr float GAP_Y = 27.f;

ModeStageSelect::ModeStageSelect()
	:_buttonServer(NEW ButtonServer())
	,_isSelect(false)
	,_bgsNum(0)
	,_selectNum(0)
{

	{
		auto targetUI = _buttonServer->GetSelectUI();

		targetUI->SetHandle(ResourceServer::LoadGraph("res/UI/StageSelect/ui_targetframe_01.png"));
		targetUI->SetSize(Vector2(384.f * SCREEN_WIDTH_MAG, 64.f * SCREEN_HEIGHT_MAG));
		targetUI->SetAlpha(1.f);
	}

	for (int a = 0; a < 3; a++) {
		std::string fileName = "res/UI/StageSelect/ui_stagebg_0";
		fileName += std::to_string(a+1);
		fileName += ".png";

		//使用する画像の描画用クラス
		SpriteText* text = NEW SpriteText();
		text->SetHandle(ResourceServer::LoadGraph(fileName.c_str()));
		text->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
		text->SetPos(Vector2(
			SCREEN_WIDTH / 2.f,
			SCREEN_HEIGHT / 2.f
		)
		);

		_bgs[a] = text;

	}

	{
		_tag = NEW SpriteText();
		_tag->SetHandle(ResourceServer::LoadGraph("res/UI/StageSelect/ui_stageselection_01.png"));
		_tag->SetSize(Vector2(650.f*SCREEN_WIDTH_MAG,120.f*SCREEN_HEIGHT_MAG));
		_tag->SetPos(Vector2(-2000.f * SCREEN_WIDTH_MAG, 45.f * SCREEN_HEIGHT_MAG));

		_tag->AddAnimation(NEW TransformAnimation(_tag, 30, Transform2(Vector2(322.f * SCREEN_WIDTH_MAG, 45.f * SCREEN_HEIGHT_MAG))));
	}
}

ModeStageSelect::~ModeStageSelect() {
	delete _buttonServer;
	delete _tag;
	for (auto&& bg : _bgs) {
		delete bg;
	}
}

bool ModeStageSelect::Initialize() {

	gGlobal._sndServer.Play("BGM_02");

	for(int stageTypeNum = 1; stageTypeNum < 4; stageTypeNum++) {

		std::string filePath = "res/UI/StageSelect/stage";
		filePath += std::to_string(stageTypeNum);
		filePath += "/";

		{
			//ステージ選択用ボタン
			std::string stageTypeFilePath = filePath + "ui_stage0";
			stageTypeFilePath += std::to_string(stageTypeNum);

			stageTypeFilePath += ".png";

			SpriteText* stageTypeText = NEW SpriteText();
			stageTypeText->SetHandle(ResourceServer::LoadGraph(stageTypeFilePath.c_str()));
			stageTypeText->SetSize(Vector2(600.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG));
			stageTypeText->SetPos(Vector2(
				-2000.f * SCREEN_WIDTH_MAG,
				300.f * SCREEN_HEIGHT_MAG + ((100.f + GAP_Y) * SCREEN_HEIGHT_MAG) * stageTypeNum)
			);

			//移動用のアニメーション
			stageTypeText->AddAnimation(NEW TransformAnimation(
				stageTypeText,
				30,
				Transform2(
					Vector2(
						331.5f * SCREEN_WIDTH_MAG,
						300.f * SCREEN_HEIGHT_MAG + ((100.f + GAP_Y) * SCREEN_HEIGHT_MAG) * stageTypeNum)
					)
				)
			);

			auto stageTypefunc = [=]()mutable {

				//ステージ選択用ボタンを画面外へ移動
				for (auto&& button : _buttonServer->GetButtons()) {
					button->GetSpriteText()->Reverse();


					button->GetSpriteText()->GetAnimations().front()->SetFunc(
						[=]()mutable {
						_buttonServer->DeleteButton(button);
						}
					);
				}

				//ModeGame(ゲーム本編)遷移用ボタン
				for (int stageNum = 1; stageNum < 4; stageNum++) {

					//使用する画像のファイルパス
					std::string stageFilePath = filePath + "ui_stage0";
					stageFilePath += std::to_string(stageTypeNum);
					stageFilePath += "_";
					stageFilePath += std::to_string(stageNum);
					stageFilePath += ".png";

					//使用する画像の描画用クラス
					SpriteText* text = NEW SpriteText();
					text->SetHandle(ResourceServer::LoadGraph(stageFilePath.c_str()));
					text->SetSize(Vector2(600.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG));
					text->SetPos(Vector2(
						-2000.f * SCREEN_WIDTH_MAG,
						300.f * SCREEN_HEIGHT_MAG + ((100.f + GAP_Y) * SCREEN_HEIGHT_MAG) * stageNum)
					);

					//移動用のアニメーション
					text->AddAnimation(NEW TransformAnimation(
						text,
						30,
						Transform2(
							Vector2(
								331.5f * SCREEN_WIDTH_MAG,
								300.f * SCREEN_HEIGHT_MAG + ((100.f + GAP_Y) * SCREEN_HEIGHT_MAG) * stageNum)
						)
					)
					);

					//押された時の処理
					auto func = [=]()mutable {
						//
						auto mode = ModeServer::GetInstance();


						auto fadeFunc = [=]()mutable {
							mode->Del(this);
							std::string stageName = std::to_string(stageTypeNum) + "_" + std::to_string(stageNum); 
							mode->Add(NEW ModeGame(stageName), 1, "game");
						};

							mode->Add(NEW ModeColorOut(NEW ModeColorIn(60,true), fadeFunc, 60), 100, "Out");

					
					};

					_buttonServer->AddButton(NEW Button(_buttonServer, func, text));

				}

				//ステージ選択用ボタンに戻る
				{
					auto backButtoText = NEW SpriteText();
					backButtoText->SetHandle(ResourceServer::LoadGraph("res/UI/StageSelect/ui_back_01.png"));
					backButtoText->SetSize(Vector2(600.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG));
					backButtoText->SetPos(Vector2(-2000.f * SCREEN_WIDTH_MAG, 975.f * SCREEN_HEIGHT_MAG));
					//移動用のアニメーション
					backButtoText->AddAnimation(NEW TransformAnimation(
						backButtoText,
						30,
						Transform2(
							Vector2(300.f * SCREEN_WIDTH_MAG, 975.f * SCREEN_HEIGHT_MAG)
						)
					)
					);

					auto backButtonFunc = [=]()mutable {
						//ステージ選択用ボタンを画面外へ移動
						for (auto&& button : _buttonServer->GetButtons()) {
							button->GetSpriteText()->Reverse();

							auto animFunc = [=]()mutable {
								_buttonServer->DeleteButton(button);
							};

							button->GetSpriteText()->GetAnimations().front()->SetFunc(animFunc);

			
						}

						Initialize();
						_buttonServer->SetSelectNum(_selectNum);

					};

					_buttonServer->AddButton(NEW Button(_buttonServer, backButtonFunc, backButtoText));
				}
				_buttonServer->SetSelectNum(0);
			};

			_buttonServer->AddButton(NEW Button(_buttonServer, stageTypefunc, stageTypeText));
		}
	}

	//タイトルに戻るボタン
	{
		auto backTitleButtoText = NEW SpriteText();
		backTitleButtoText->SetHandle(ResourceServer::LoadGraph("res/UI/StageSelect/ui_totitle_01.png"));
		backTitleButtoText->SetSize(Vector2(600.f * SCREEN_WIDTH_MAG, 100.f * SCREEN_HEIGHT_MAG));
		backTitleButtoText->SetPos(Vector2(-2000.f * SCREEN_WIDTH_MAG, 975.f * SCREEN_HEIGHT_MAG));
		//移動用のアニメーション
		backTitleButtoText->AddAnimation(NEW TransformAnimation(
			backTitleButtoText,
			30,
			Transform2(
				Vector2(300.f * SCREEN_WIDTH_MAG, 975.f * SCREEN_HEIGHT_MAG)
			)
		)
		);

		auto backTitleButtonFunc = [=]()mutable {

			auto mode = ModeServer::GetInstance();


			auto fadeFunc = [=]()mutable {				
				mode->Del(this);
				mode->Add(NEW ModeTitle(), 1, "title");
			};

			mode->Add(NEW ModeColorOut(NEW ModeColorIn(60,true), fadeFunc, 60), 100, "Out");

		};
		_buttonServer->AddButton(NEW Button(_buttonServer, backTitleButtonFunc, backTitleButtoText));
	}

	return true;
}

bool ModeStageSelect::Terminate() {

	return true;
}

bool ModeStageSelect::Process() {
	_buttonServer->Process();
	if (ApplicationMain::GetInstance()->GetPad()->GetTrgButton() & INPUT_A) {
		_isSelect = !_isSelect;
	}
	if (!_isSelect && 3 > _buttonServer->GetSelectNum()) {
		_bgsNum = _buttonServer->GetSelectNum();
		_selectNum = _buttonServer->GetSelectNum();
	}


	return true;
}

bool ModeStageSelect::Render() {
	_bgs[_bgsNum]->Draw();
	_tag->Draw();
	_buttonServer->Draw();

	return true;
}

