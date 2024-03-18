#include"ButtonServer.h"
#include"Button.h"

#include"SpriteText.h"
#include"Animation.h"

#include<algorithm>
ButtonServer::ButtonServer() 
	:_selectNum(0)
	,_selectLimitNum(-1)
	,_oldSelectNum(0)
	,_selectUI(NEW SpriteText())
	,_step(STEP::kAnimation)
	,_isProcess(false)
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

	_isProcess = true;

	if (!_addButtons.empty()) {
		for (auto&& button : _addButtons) {
			_buttons.emplace_back(button);
		}
		_addButtons.clear();
	}

	if (!_deleteButtons.empty()) {
		for (auto&& button : _deleteButtons) {
			auto iter = std::find(_buttons.begin(), _buttons.end(), button);

			if (iter != _buttons.end()) {
				delete (*iter);
				_buttons.erase(iter);
			}
		}
		_deleteButtons.clear();
	}

	//サイズを超えていたら、修正
	if (_selectNum >= _buttons.size()) { _selectNum = _buttons.size() - 1; }

	if (_selectLimitNum != -1 && _selectNum > _selectLimitNum - 1) {
		_selectNum = _selectLimitNum;
	}

	//空だったら処理をしない
	if (_buttons.empty()) { return true; }

	switch(_step) {
	case STEP::kAnimation:
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

		_oldSelectNum = _selectNum;

		//選択ボタンの変更
		if(trg & INPUT_DPAD_DOWN) {
			_selectNum++;
		}
		else if(trg & INPUT_DPAD_UP) {
			_selectNum--;
			if (_selectNum < 0)_selectNum = _buttons.size() - 1;
		}
		_selectNum = _selectNum % _buttons.size();

		
		//ボタンが押された時の処理
		if (trg & INPUT_A) {
			_buttons[_selectNum]->SelectFunc();
		}

		bool isPlayAnimation = false;
		for (auto&& button : _buttons) {

			for (auto&& anim : button->GetSpriteText()->GetAnimations()) {
				if (!anim->IsEnd()) {
					isPlayAnimation = true; break;
				}
			}

		}

		if (isPlayAnimation) {
			_step = STEP::kAnimation;
		}

		break;
	}
	}

	_selectUI->SetTransform(_buttons[_selectNum]->GetSpriteText()->GetTransform());
	_selectUI->SetSize(_buttons[_selectNum]->GetSpriteText()->GetSize());

	_isProcess = false;
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
	auto iter = std::find(_buttons.begin(), _buttons.end(), button);

	if (iter != _buttons.end())return;

	iter = std::find(_addButtons.begin(), _addButtons.end(), button);

	if (iter != _addButtons.end())return;

	if (!_isProcess) {
		_buttons.emplace_back(button);
		return;
	}

	_addButtons.emplace_back(button);
}

void ButtonServer::DeleteButton(Button* button) {

	auto iter = std::find(_buttons.begin(), _buttons.end(), button);

	if(iter == _buttons.end()) {
		return;
	}

	if (!_isProcess) {
		_buttons.erase(iter);
		return;
	}

	_deleteButtons.emplace_back(button);

}