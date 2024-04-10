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

	//���U���g�f�[�^
	std::shared_ptr<class ResultData> _resultData;

	//�{�^���T�[�o�[
	class ButtonServer* _buttonServer;

	//UI�T�[�o�[
	class MyUIServer* _uiServer;

	//���Ԍo�߂ɂ���ēo�^���ꂽ�����_������������
	class TimeLine* _timeLine;
};