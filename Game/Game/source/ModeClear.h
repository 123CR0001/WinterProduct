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

	//リザルトデータ
	std::shared_ptr<class ResultData> _resultData;

	//ボタンサーバー
	class ButtonServer* _buttonServer;

	//UIサーバー
	class MyUIServer* _uiServer;

	//時間経過によって登録されたラムダ式を処理する
	class TimeLine* _timeLine;
};