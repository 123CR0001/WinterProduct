#pragma once
#include"OrbitCamera.h"

class ModeDebugMenu :public ModeBase {
public:
	ModeDebugMenu(class ModeGame* game):_game(game){}
	~ModeDebugMenu();
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void RenderCharaCollision();
	void RenderEnemyRoot();
	void RenderVisionCollision();
	void RenderEnemyAIName();
	void RenderNavigationPolygons();
	void RenerSirenInfo();

	class ModeGame* GetGame()const { return _game; }

	static bool _charaCollision;
	static bool _root;
	static bool _visionCollision;
	static bool _AIName;
	static bool _cameraChange;
	static bool _navigationPolygon;
	static bool _sirenInfo;

protected:
	class ModeGame* _game;

	//メニューの一番上から描画するUIの番号
	int _start;
	//DebuMenuで選択するか
	bool _isProcess;

	//入力情報
	int _key;

	//入力されたフレーム
	int _inputFrameCnt;

	//選んでいるUIの番号
	int _selectNum;
	//メニューに描画するUIの個数
	int _drawNum;
public:
	//サブの人が制作したUIBaseとかぶらないようにするため
	class DebugMenuItem {
	public:
		DebugMenuItem(class ModeDebugMenu* menu) :_menu(menu) {}
		virtual ~DebugMenuItem() {}

		virtual const char* GetText() = 0;

		virtual void Select() = 0;

		class ModeDebugMenu* _menu;
	};
protected:

	std::vector<DebugMenuItem*>_debugMenus;

};