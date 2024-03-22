#pragma once
#include"appframe.h"

class ModeStageSelect :public ModeBase {
public:

	ModeStageSelect();
	~ModeStageSelect();
	
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

private:
	class ButtonServer* _buttonServer;
	bool _isSelect;

	std::array<class SpriteText*, 3>_bgs;
	//�`�悷��w�i�̔ԍ�
	int _bgsNum;

	class SpriteText* _tag;

	int _selectNum;
};