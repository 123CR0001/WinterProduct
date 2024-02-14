
#include "ApplicationMain.h"
#include "ModeSelect.h"

#include "UIServer.h"
#include "UIDisplay.h"
#include "UISlide.h"
#include "UISelectController.h"
#include "UITitle.h"

bool ModeSelect::Initialize() {
	if (!base::Initialize()) { return false; }

	_ui = new UIServer();
	_ui->Initialize();

	Load();
	
	_ui->Search("stage1")->_selectNum = 0;
	_ui->Search("stage2")->_selectNum = 1;
	_ui->Search("stage3")->_selectNum = 2;
	_ui->Search("toTitle")->_selectNum = 3;

	_select = _ui->Search("stage1")->_selectNum;

	_maxOptions = STAGE_SELECT_NUM;

	_bSelected = false;

	return true;
}

bool ModeSelect::Terminate() {
	base::Terminate();
	_ui->Terminate();
	delete _ui;
	return true;
}

bool ModeSelect::Process() {
	base::Process();

	int trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	_ui->Process();

	if (_bSelected) { _maxOptions = AREA_SELECT_NUM; }
	else { _maxOptions = STAGE_SELECT_NUM; }

	// 上下でカーソルを移動する
	if (trg & INPUT_DPAD_UP) { _select--; }
	if (trg & INPUT_DPAD_DOWN) { _select++; }

	// 選択位置を上下ループ
	_select = (_select + _maxOptions) % _maxOptions;


	// 選択処理
	for (const auto& ui : _ui->_vUI) {
		if (ui == _ui->Search("Get")) { continue; }			// 
		if (_bSelected && trg & INPUT_B && ui->_selectNum == 0) {
			ui->Selected();
			_bSelected = false;
			_select = 0;
		}
		if (ui->_selectNum == _select) {					// ui側に割り振られた番号と_selectの数字が同じか否か
			std::string name = ui->_uiName;					// 選択している項目の名前
			SwitchTagWithBg(name);
			AdjustItemPos(name);
			_ui->SelectPosAdjustment(name, "Get", 0, -5);	// 選択中の項目にカーソルを重ねる	
			if (trg & INPUT_A) {
				ui->Selected();
				if (_bSelected) { _bSelected = false; }
				else { _bSelected = true; _select = 0; }
				
			}
			break;
		}
	}


	return true;
}

bool ModeSelect::Render() {
	base::Render();

	float oldW = 0, oldY = 0, oldH = 0;
	std::string selectName;
	for (const auto& ui : _ui->_vUI) {
		if (ui == _ui->Search("Get")) { continue; }
		if (ui->_selectNum == _select) {
			selectName = ui->_uiName;
			oldW = ui->_w;
			oldY = ui->_y;
			oldH = ui->_h;
			ui->_w += 51;
			ui->_y -= 5;
			ui->_h += 10;
			break;
		}
	}

	_ui->Render();

	// ループ外で選択されたUI要素を取得
	auto selectUI = _ui->Search(selectName);
	
	// 元の値に戻す
	selectUI->_w = oldW;
	selectUI->_y = oldY;
	selectUI->_h = oldH;

	{
		std::string name;
		for (const auto& ui : _ui->_vUI) {
			if (ui == _ui->Search("Get")) { continue; }			// 
			if (ui->_selectNum == _select) {					// ui側に割り振られた番号と_selectの数字が同じか否か
				name = ui->_uiName;					// 選択している項目の名前	
				break;
			}
		}

		int ss = _ui->Search("toTitle")->_selectNum;
		int X = _ui->Search("Get")->_x;
	int Y = _ui->Search("Get")->_y;
		DrawFormatString(0, 0, GetColor(255, 0, 0), "X:%d \nY:%d \n%d \n%s \n%d", X, Y, _select, name.c_str(), ss);
	}

	return true;
}

void ModeSelect::Load() {
	// 背景
	std::vector<std::string> bgImages = {
		"res/UI/StageSelect/ui_stagebg_01.png",
		"res/UI/StageSelect/ui_stagebg_02.png",
		"res/UI/StageSelect/ui_stagebg_03.png"
	};

	for (size_t i = 0; i < bgImages.size(); ++i) {
		_ui->Add(new UIDisplay(), nullptr, res::LoadGraph(bgImages[i].c_str()), 0, 0, 1920, 1080, 1, "bgStage" + std::to_string(i + 1));
	}

	// フレーム
	std::vector<std::string> frameImages = {
		"res/UI/StageSelect/ui_stageframe_01.png",
		"res/UI/StageSelect/ui_stageframe_02.png"
	};

	for (size_t i = 0; i < frameImages.size(); ++i) {
		float initX = i == 0 ? -800 : 1920;
		float endX = i == 0 ? 0 : 800;
		float w = i == 0 ? 800 : 1120;
		_ui->Add(new UISlide(initX, 0, endX, 0, 20), nullptr,
			res::LoadGraph(frameImages[i].c_str()), initX, 0, w, 1080, LOW_LAYER_VALUE, i == 0 ? "leftFrame" : "rightFrame");
	}

	// タグ
	std::vector<std::string> tagImages = {
		"res/UI/StageSelect/ui_stageselection_01.png",
		"res/UI/StageSelect/ui_stagetag_01.png",
		"res/UI/StageSelect/ui_stagetag_02.png",
		"res/UI/StageSelect/ui_stagetag_03.png"
	};

	std::vector<std::string> tagNames = {
		"tagSystem",
		"tagStage1",
		"tagStage2",
		"tagStage3"
	};

	for (size_t i = 0; i < tagImages.size(); ++i) {
		float initX = i == 0 ? -800 : 2592;
		float initY = i == 0 ? 5 : 64;
		float endX = i == 0 ? 0 : 1472;
		float endY = i == 0 ? 5 : 64;
		float w = i == 0 ? 650 : 450;
		_ui->Add(new UISlide(initX, initY, endX, endY, 20), nullptr,
			res::LoadGraph(tagImages[i].c_str()), initX, initY, w, 120, BASIC_LAYER_VALUE, tagNames[i]);
		if (i > 1) {
			_ui->Search(tagNames[i])->_bView = false;
		}
	}

	// ステージセレクト 
	std::vector<std::string> stageImages = {
		"res/UI/StageSelect/stage1/ui_stage01.png",
		"res/UI/StageSelect/stage2/ui_stage02.png",
		"res/UI/StageSelect/stage3/ui_stage03.png",
		"res/UI/StageSelect/ui_totitle_01.png",
		"res/UI/StageSelect/ui_targetframe_01.png"
	};

	std::vector<std::string> stageNames = {
		"stage1",
		"stage2",
		"stage3",
		"toTitle",
		"Get"
	};

	std::vector<std::pair<float, float>> stagePos = {
		{-770, 192},
		{-770, 320},
		{-770, 448},
		{-770, 925},
		{-770, 192}
	};

	std::vector<std::pair<float, float>> stageSizes = {
		{600, 100},
		{600, 100},
		{600, 100},
		{512, 100},
		{10, 110}
	};

	for (size_t i = 0; i < stageImages.size(); ++i) {
		float initX = stagePos[i].first;
		float initY = stagePos[i].second;
		float endY = stagePos[i].second;
		float w = stageSizes[i].first;
		float h = stageSizes[i].second;
		if (i == 3) {
			_ui->Add(new UITitle(initX, initY, 30, endY, 20, "select"), _ui,
				res::LoadGraph(stageImages[i].c_str()), initX, initY, w, h, BASIC_LAYER_VALUE, stageNames[i]);
			continue;
		}
		else {
			_ui->Add(new UISelectController(initX, initY, 30, endY, 20), _ui,
				res::LoadGraph(stageImages[i].c_str()), initX, initY, w, h, BASIC_LAYER_VALUE, stageNames[i]);
		}
	}
}

bool ModeSelect::SwitchTagWithBg(std::string selecting) {
	if (selecting == "toTitle") { return false; }

	std::vector<std::string> tags = { "tagStage1", "tagStage2", "tagStage3" };
	std::vector<std::string> backgrounds = { "bgStage1", "bgStage2", "bgStage3" };

	
	for (const auto& tag : tags) {
		_ui->Search(tag)->_bView = false;
	}
	for (const auto& bg : backgrounds) {
		_ui->Search(bg)->_bView = false;
	}

	int index = std::stoi(selecting.substr(5)) - 1;
	if (index >= 0 && index < tags.size()) {
		_ui->Search(tags[index])->_bView = true;
		_ui->Search(backgrounds[index])->_bView = true;
	}

	return true;
}

bool ModeSelect::AdjustItemPos(std::string selecting) {
	if(selecting == "toTitle") { return false; }

	float stage1Y = _ui->Search("stage1")->_y;
	float stage2Y = _ui->Search("stage2")->_y;
	float stage3Y = _ui->Search("stage3")->_y;

	if (_ui->Search(selecting) == _ui->Search("stage1")) {
		stage2Y += 5;
		stage3Y += 5;
	}
	else if (_ui->Search(selecting) == _ui->Search("stage2")) {
		stage1Y -= 5;
		stage3Y += 5;
	}
	else if (_ui->Search(selecting) == _ui->Search("stage3")) {
		stage1Y -= 5;
		stage2Y -= 5;
	}

	_ui->Search("stage1")->_y = stage1Y;
	_ui->Search("stage2")->_y = stage2Y;
	_ui->Search("stage3")->_y = stage3Y;

	return true;
}
