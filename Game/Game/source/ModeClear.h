#pragma once
#include "appframe.h"

class ModeClear :public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;
public:
	ModeClear(std::shared_ptr<class ClearData> data);

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:

	void SetUI();
	void SetButton();

	std::shared_ptr<class ClearData> _clearData;			//リザルトデータ

	class ButtonServer* _buttonServer;						//ボタンサーバー	

	class UIScreen* _uiScreen;							//UIサーバー

	class TimeLine* _timeLine;								//時間経過によって登録されたラムダ式を処理するクラス
};