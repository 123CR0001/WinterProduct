#pragma once
#include"appframe.h"
#include<memory>

class ModeTutorial :public ModeBase {
public:
	ModeTutorial(std::string name);

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

private:

	class SpriteText* _text;	//描画するチュートリアルのテキスト

	//このモードを終了するか
	bool _isEnd;
};