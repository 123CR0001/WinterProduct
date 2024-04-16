#pragma once
#include "appframe.h"

class ModeClear :public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;
public:
	ModeClear(std::shared_ptr<class ResultData> data);

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:

	void SetUI();
	void SetButton();

	std::shared_ptr<class ResultData> _resultData;			//���U���g�f�[�^

	class ButtonServer* _buttonServer;						//�{�^���T�[�o�[	

	class UIScreen* _uiServer;							//UI�T�[�o�[

	class TimeLine* _timeLine;								//���Ԍo�߂ɂ���ēo�^���ꂽ�����_������������N���X
};