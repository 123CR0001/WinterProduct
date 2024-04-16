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

	class SpriteText* _text;	//�`�悷��`���[�g���A���̃e�L�X�g

	//���̃��[�h���I�����邩
	bool _isEnd;
};