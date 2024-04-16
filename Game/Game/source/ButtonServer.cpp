#include"ButtonServer.h"
#include"Button.h"

#include"SpriteText.h"
#include"Animation.h"

#include<algorithm>
ButtonServer::ButtonServer() 
	:_selectNum(0)
	,_selectUI(NEW SpriteText())
	,_step(STEP::kAnimation)
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
	if (_selectNum >= static_cast<int>(_buttons.size())) { 
		_selectNum = static_cast<int>(_buttons.size()) - 1;
	}

	//空だったら処理をしない
	if (_buttons.empty()) { return true; }

	switch(_step) {
	case STEP::kAnimation:
	{
		//アニメーションが終了したか
		bool isPlayAnimation = false;
		for(auto&& button : _buttons) {
			
			for (auto&& anim : button->GetSpriteText()->GetAnimations()) {
				if (!anim->IsEnd()) {
					isPlayAnimation = true; break;
				}
			}

		}

		//アニメーションが終了したら次のステップへ
		if (!isPlayAnimation) {
			_step = STEP::kProcess;
		}
		break;
	}
	case STEP::kProcess:
	{
		//トリガ入力
		auto trg = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

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

		//アニメーションがさらに再生される場合、アニメーションステップへ
		{
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
		}
		break;
	}
	}

	//選択UIの位置を変更
	_selectUI->SetTransform(_buttons[_selectNum]->GetSpriteText()->GetTransform());
	_selectUI->SetSize(_buttons[_selectNum]->GetSpriteText()->GetSize());

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

	//重複登録を防ぐ
	auto iter = std::find(_buttons.begin(), _buttons.end(), button);

	if (iter != _buttons.end())return;

	iter = std::find(_addButtons.begin(), _addButtons.end(), button);

	if (iter != _addButtons.end())return;

	_addButtons.emplace_back(button);
}

void ButtonServer::DeleteButton(Button* button) {

	//重複登録を防ぐ
	auto iter = std::find(_buttons.begin(), _buttons.end(), button);

	if(iter == _buttons.end()) {
		return;
	}

	//削除リストに追加
	_deleteButtons.emplace_back(button);

}