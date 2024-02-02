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

	//���j���[�̈�ԏォ��`�悷��UI�̔ԍ�
	int _start;
	//DebuMenu�őI�����邩
	bool _isProcess;

	//���͏��
	int _key;

	//���͂��ꂽ�t���[��
	int _inputFrameCnt;

	//�I��ł���UI�̔ԍ�
	int _selectNum;
	//���j���[�ɕ`�悷��UI�̌�
	int _drawNum;
public:
	//�T�u�̐l�����삵��UIBase�Ƃ��Ԃ�Ȃ��悤�ɂ��邽��
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