#include"ButtonServer.h"
#include"Button.h"

#include"SpriteText.h"
#include"Animation.h"

#include<algorithm>
ButtonServer::ButtonServer() 
	:_selectNum(0)
	,_oldSelectNum(0)
	,_trgSelectNum(0)
	,_relSelectNum(0)
	,_selectUI(NEW SpriteText())
	,_step(STEP::kIntroduct)
{
}

ButtonServer::~ButtonServer(){

	for (auto&& button : _buttons) {
		delete button;
	}

	_buttons.clear();

	delete _selectUI;

}

bool ButtonServer::Process() {

	switch(_step) {
	case STEP::kIntroduct:
	{
		bool isPlayAnimation = false;
		for(auto&& button : _buttons) {
			
			for (auto&& anim : button->GetSpriteText()->GetAnimations()) {
				if (!anim->IsEnd()) {
					isPlayAnimation = true; break;
				}
			}

		}

		if (!isPlayAnimation) {
			_step = STEP::kProcess;
		}
		break;
	}
	case STEP::kProcess:
	{
		auto trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

		_trgSelectNum = -1;
		_relSelectNum = -1;

		_oldSelectNum = _selectNum;

		//選択ボタンの変更
		if(trg & INPUT_DPAD_DOWN) {
			_selectNum--;
		}
		else if(trg & INPUT_DPAD_UP) {
			_selectNum++;
		}
		_selectNum %= _buttons.size();

		//
		if ((_selectNum ^ _oldSelectNum) & ~_oldSelectNum) {
			_trgSelectNum = _selectNum;
		}
		if ((_selectNum ^ _oldSelectNum) & _oldSelectNum) {
			_relSelectNum = _oldSelectNum;
		}
		
		//ボタンが押された時の処理
		if (trg & INPUT_A) {
			_buttons[_selectNum]->SelectFunc();

			_step = STEP::kConclude;
		}

		break;
	}
	case STEP::kConclude:
	{

		break;
	}
	}

	_selectUI->SetTransform(_buttons[_selectNum]->GetSpriteText()->GetTransform());

	return true;
}

bool ButtonServer::Draw() {
	for(auto&& button : _buttons) {
		button->Draw();
	}
	_selectUI->Draw();
	return true;
}

void ButtonServer::AddButton(Button* button) {
	_buttons.emplace_back(button);
}

void ButtonServer::DeleteButton(Button* button) {

	auto iter = std::find(_buttons.begin(), _buttons.end(), button);

	if(iter != _buttons.end()) {
		delete button;
		_buttons.erase(iter);
	}



}