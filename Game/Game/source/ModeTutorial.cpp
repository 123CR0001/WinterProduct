#include"ModeTutorial.h"
#include"UIScreen.h"
#include"UI.h"
#include"SpriteText.h"
#include"ApplicationMain.h"
#include"UISpriteText.h"

#include"TransformAnimation.h"

ModeTutorial::ModeTutorial(std::string name) 
	:_isEnd(false)
{

	//ファイルパスの作成
	std::string filePath = "res/UI/Tutorial/";
	filePath += name;
	filePath += ".png";

	//テキストの作成
	Transform2 transform(Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT*2));
	Vector2 size(SCREEN_WIDTH, SCREEN_HEIGHT);

	_text = NEW SpriteText(ResourceServer::LoadGraph(filePath.c_str()), transform, size);

	Transform2 endTransform(Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));

	_text->AddAnimation(NEW TransformAnimation(_text, 60, endTransform));

}

bool ModeTutorial::Initialize() {
	return true;
}

bool ModeTutorial::Terminate() {

	delete _text;

	return true;
}

bool ModeTutorial::Process() {

	auto pad = ApplicationMain::GetInstance()->GetPad()->GetTrgButton();

	// Aボタンが押されたらテキストを消す
	if (pad & INPUT_A && _text->IsAllAnimationEnd() && !_isEnd) {
		_text->Reverse();
		_isEnd = true;
	}

	// テキストのアニメーションが全て終了したら、モードを削除
	if (_isEnd && _text->IsAllAnimationEnd()) {
		ModeServer::GetInstance()->Del(this);
	}


	// このモードよりレイヤーが低いものは処理を止める
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	return true;
}

bool ModeTutorial::Render() {
	_text->Draw();
	return true;
}