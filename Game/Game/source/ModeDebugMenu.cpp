#include"ModeDebugMenu.h"
#include "ApplicationMain.h"
#include"CommonSoldier.h"
#include"AIBackPatrol.h"
#include"AIComponent.h"
#include"AIPatrol.h"
#include"Player.h"
#include"Siren.h"
#include"ObjectServer.h"
#include"FollowCamera.h"

#include"CapsuleComponent.h"
#include"SoundComponent.h"

#include"ModeEffekseer.h"
#include"ModeGame.h"

#include"appframe.h"

bool ModeDebugMenu::_charaCollision = false;
bool ModeDebugMenu::_root = false;
bool ModeDebugMenu::_visionCollision = false;
bool ModeDebugMenu::_AIName = false;
bool ModeDebugMenu::_cameraChange = false;
bool ModeDebugMenu::_navigationPolygon = false;
bool ModeDebugMenu::_sirenInfo = false;
bool ModeDebugMenu::_soundInfo = false;
bool ModeDebugMenu::_detectionInfo = false;

constexpr int FONT_SIZE = 20;
constexpr int MENU_HEIGHT = 500;
constexpr int FITS_MENU_UI_NUM = MENU_HEIGHT / FONT_SIZE;

class CharaCollision :public ModeDebugMenu::DebugMenuItem {
public:
	CharaCollision(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "キャラクターのコリジョンを表示"; }
	void Select()override { ModeDebugMenu::_charaCollision = !ModeDebugMenu::_charaCollision; }
};

class CommonSoldierRoot :public ModeDebugMenu::DebugMenuItem {
public:
	CommonSoldierRoot(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "エネミーの巡回経路を表示"; }
	void Select()override { ModeDebugMenu::_root = !ModeDebugMenu::_root; }
};

class CommonSolidierVisionCollision : public ModeDebugMenu::DebugMenuItem {
public:
	CommonSolidierVisionCollision(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "エネミーの視界範囲を表示"; }
	void Select()override { ModeDebugMenu::_visionCollision = !ModeDebugMenu::_visionCollision; }
};

class CommonSoldierAIName :public ModeDebugMenu::DebugMenuItem {
public:
	CommonSoldierAIName(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "エネミーのAIの名前"; }
	void Select()override { ModeDebugMenu::_AIName = !ModeDebugMenu::_AIName; }
};

class PlayerCameraChange : public ModeDebugMenu::DebugMenuItem {
public:
	PlayerCameraChange(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "カメラの処理を切り替え"; }
	void Select()override {
		ModeDebugMenu::_cameraChange = !ModeDebugMenu::_cameraChange;
		if (ModeDebugMenu::_cameraChange) {
			auto player = _menu->GetGame()->GetObjectServer()->GetPlayer();

			player->DeleteComponent(player->GetCamera());

			camera = NEW OrbitCamera(player, 10000);
			player->SetCamera(camera);
		}
		else {
			auto player = _menu->GetGame()->GetObjectServer()->GetPlayer();
			player->SetCamera(NEW FollowCamera(player,1000));
			player->DeleteComponent(camera);
			camera = nullptr;
		}
	}

	CameraComponent* camera = nullptr;
	CameraComponent* playerCamera;
};

class NavigationPolygons :public ModeDebugMenu::DebugMenuItem {
public:
	NavigationPolygons(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "ナビゲーション用のポリゴンを描画"; }
	void Select()override {
		ModeDebugMenu::_navigationPolygon = !ModeDebugMenu::_navigationPolygon;
	}
};

class GameEnd :public ModeDebugMenu::DebugMenuItem {
public:
	GameEnd(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "ゲームを終了"; }
	void Select()override {
		ApplicationBase::GetInstance()->GameEnd();
	}
};

class PlayEffect :public ModeDebugMenu::DebugMenuItem {
public:
	PlayEffect(ModeDebugMenu* menu,std::string effectName) :DebugMenuItem(menu) ,_effectName(effectName){}
	const char* GetText()override { return _effectName.c_str(); }
	void Select()override {
		auto* player = _menu->GetGame()->GetObjectServer()->GetPlayer();
		_menu->GetGame()->GetModeEffekseer()->Play(
			_effectName,
			player->GetPos()+Vector3(0.f,180.f,0.f), 
			player->GetEulerAngle()
		);
	}
protected:
	std::string _effectName;
};

class StageReset :public ModeDebugMenu::DebugMenuItem {
public:
	StageReset(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	~StageReset() {}
	const char* GetText()override { return "キャラやステージの状態を初期化"; }
	void Select()override {
		_menu->GetGame()->Terminate();
		_menu->GetGame()->Initialize();
	}
};

class SirenInfo :public ModeDebugMenu::DebugMenuItem {
public:
	SirenInfo(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	~SirenInfo(){}
	const char* GetText()override { return "サイレンの情報を描画"; }
	void Select()override {
		ModeDebugMenu::_sirenInfo = !ModeDebugMenu::_sirenInfo;
	}
};

class SoundInfo :public ModeDebugMenu::DebugMenuItem {
public:
	SoundInfo(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	~SoundInfo() {}
	const char* GetText()override { return "サウンドコンポーネントの情報を描画"; }
	void Select()override {
		ModeDebugMenu::_soundInfo = !ModeDebugMenu::_soundInfo;
	}
};

class DetectionInfo :public ModeDebugMenu::DebugMenuItem {
public:
	DetectionInfo(ModeDebugMenu* menu) : DebugMenuItem(menu) {}
	~DetectionInfo(){}
	const char* GetText()override { return "検知度を描画"; }
	void Select()override {
		ModeDebugMenu::_detectionInfo = !ModeDebugMenu::_detectionInfo;
	}
};

//ModeDebugMenu::ModeDebugMenu(ModeGame* game)
//	:_game(game) {
//
//}

ModeDebugMenu::~ModeDebugMenu(){
	Terminate();
}

bool ModeDebugMenu::Initialize() {
	_selectNum = 0;
	_isProcess = false;
	_inputFrameCnt = 0;
	_start = 0;

	_debugMenus.emplace_back(NEW CharaCollision(this));
	_debugMenus.emplace_back(NEW CommonSoldierRoot(this));
	_debugMenus.emplace_back(NEW CommonSolidierVisionCollision(this));
	_debugMenus.emplace_back(NEW CommonSoldierAIName(this));
	_debugMenus.emplace_back(NEW PlayerCameraChange(this));
	_debugMenus.emplace_back(NEW NavigationPolygons(this));
	_debugMenus.emplace_back(NEW GameEnd(this));
	_debugMenus.emplace_back(NEW StageReset(this));
	_debugMenus.emplace_back(NEW SirenInfo(this));
	_debugMenus.emplace_back(NEW SoundInfo(this));
	_debugMenus.emplace_back(NEW DetectionInfo(this));
	//_UIs.emplace_back(new PlayEffect(this,"Laser"));

	//for (int a = 0; a < FITS_MENU_UI_NUM; a++) {
	//	_UIs.emplace_back(new CommonSoldierAIName());
	//	_UIs.emplace_back(new CommonSolidierVisionCollision());
	//}

	_drawNum = (int)_debugMenus.size();
	if (_drawNum > FITS_MENU_UI_NUM) { _drawNum = FITS_MENU_UI_NUM; }
	return true;
}

bool ModeDebugMenu::Terminate() {
	for (auto iter = _debugMenus.begin(); iter != _debugMenus.end(); ++iter) {
		delete (*iter);
	}
	_debugMenus.clear();
	return true;
}

bool ModeDebugMenu::Process() {
	auto pad = _game->GetPad();
	auto trg = pad->GetTrgButton();

	int OldKey = _key;

	auto _key = pad->GetKeyButton();
	if (_isProcess) {

		if (_key & INPUT_DPAD_UP) {
			if (_inputFrameCnt > 10 || _inputFrameCnt == 0) {
				_selectNum -= 1;
				if (_selectNum < 0) { _selectNum = 0; }
			}
			_inputFrameCnt++;

		}

		if (_key & INPUT_DPAD_DOWN) {
			if (_inputFrameCnt > 10 || _inputFrameCnt == 0) {
				_selectNum += 1;
				if (_selectNum >= (int)_debugMenus.size()) { _selectNum = (int)_debugMenus.size() - 1; }
			}
			_inputFrameCnt++;

		}

		if (_key == OldKey) { _inputFrameCnt = 0; }

		if (trg & INPUT_A) { _debugMenus[_selectNum]->Select(); }

		//// このモードより下のレイヤーはProcess()を呼ばない
		//ModeServer::GetInstance()->SkipProcessUnderLayer();
	}

	if (trg & INPUT_BACK) {
		_isProcess = !_isProcess;
	}


	return _isProcess;
}

bool ModeDebugMenu::Render() {

	if (_charaCollision) { RenderCharaCollision(); }
	if (_root) { RenderEnemyRoot(); }
	if (_visionCollision) { RenderViewCollision(); }
	if (_AIName) { RenderEnemyAIName(); }
	if (_navigationPolygon) { RenderNavigationPolygons(); }
	if (_sirenInfo) { RenerSirenInfo(); }
	if (_soundInfo) { RenderSoundInfo(); }
	if (_detectionInfo) { RenderDetectionInfo(); }

	DrawFormatString(0, 500, GetColor(255, 0, 0), "AI %d", _debugAIs.size());

	if (_isProcess) {
		SetFontSize(FONT_SIZE);

		DrawBox(0, 0, 500, MENU_HEIGHT, GetColor(255, 0, 0), TRUE);

		if (_selectNum - _start >= FITS_MENU_UI_NUM) { _start++; }
		if (_selectNum < _start) { _start--; }

		DrawFormatString(FONT_SIZE,( _selectNum - _start)* FONT_SIZE, GetColor(0, 0, 255), "→");

		for (int a = _start; a < _start + _drawNum; a++) {
			auto pos = Vector3(50.f, (float)FONT_SIZE * (a - _start), 0.f);
			DrawFormatString((int)pos.x, (int)pos.y, GetColor(0, 0, 255), "%s", _debugMenus[a]->GetText());
		}
		SetFontSize(GetFontSize());
	}

	return true;
}

void ModeDebugMenu::RenderCharaCollision() {
	auto capsules = _game->GetObjectServer()->GetPhysWorld()->GetCapsuleComponent();

	if (capsules.size() == 0) { return; }

	//カプセルの表示
	for (auto iter = capsules.begin(); iter != capsules.end(); ++iter) {
		Capsule capsule = (*iter)->GetCapsule();

		DrawCapsule3D(
			DxConverter::VecToDx(capsule.seg.start),
			DxConverter::VecToDx(capsule.seg.end),
			capsule.radius,
			10,
			GetColor(255, 255, 255),
			GetColor(0, 0, 255),
			FALSE
		);
	}
}

void ModeDebugMenu::RenderEnemyRoot() {


	for (auto iter = _debugAIs.begin(); iter != _debugAIs.end(); ++iter) {

		VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx((*iter)->GetOwner()->GetPos() + Vector3(0, 180, 0)));

		auto AI = (*iter);

		int num = 0;

		for (auto&& route : AI->GetPoints(AI->GetCurrentState()->GetName())) {
			num++;
			DrawSphere3D(
				DxConverter::VecToDx(route + Vector3(0, 30, 0)),
				30,
				10,
				GetColor(0, 255, 0),
				GetColor(255, 255, 255),
				FALSE
			);
			DrawFormatString(
				(int)ConvWorldPosToScreenPos(DxConverter::VecToDx(route)).x,
				(int)ConvWorldPosToScreenPos(DxConverter::VecToDx(route)).y,
				GetColor(255, 255, 255),
				"%d番目 x = %3f y = %3f z = %3f",
				num, route.x, route.y, route.z
			);
			//DrawFormatString((int)pos.x, (int)pos.y, GetColor(255, 255, 255), "最大サイズ%d", AI->GetPoints(AI->GetCurrentState()->GetName()));
		}

	}
}

void ModeDebugMenu::RenderViewCollision() {
	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();

	if (CommonSoldiers.size() == 0) { return; }

	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {
		auto _pos = (*iter)->GetPos();
		auto _eulerAngle = (*iter)->GetEulerAngle();
		auto _visionAngle = (*iter)->GetAIComponent()->GetViewAngle();
		auto _visionDist = (*iter)->GetAIComponent()->GetViewDist();

		int color = GetColor(255, 0, 0);


		if ((*iter)->GetAIComponent()->IsFound((*iter)->GetObjectServer()->GetPlayer())) {
			color = GetColor(0, 0, 255);
		}

		float angle = _eulerAngle.y + DegToRad(_visionAngle / 2);

		Vector3 vDir(0, 0, 0);

		vDir.x = sin(angle) * _visionDist;
		vDir.z = cos(angle) * _visionDist;

		Vector3 right(vDir + _pos);

		angle = _eulerAngle.y - DegToRad(_visionAngle / 2);

		vDir.x = sin(angle) * _visionDist;
		vDir.z = cos(angle) * _visionDist;

		Vector3 left(vDir + _pos);

		vDir.x = sin(_eulerAngle.y) * _visionDist;
		vDir.z = cos(_eulerAngle.y) * _visionDist;

		Vector3 forward(_pos + vDir);

		DrawTriangle3D(
			DxConverter::VecToDx(_pos + Vector3(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(forward + Vector3(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(right + Vector3(0.f, 0.1f, 0.f)),
			color,
			TRUE
		);
		DrawTriangle3D(
			DxConverter::VecToDx(_pos + Vector3(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(left + Vector3(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(forward + Vector3(0.f, 0.1f, 0.f)),
			color,
			TRUE
		);

		DrawSphere3D(
			DxConverter::VecToDx(_pos + Vector3(0, 100, 0)),
			30,
			10,
			color,
			GetColor(255, 255, 255),
			FALSE
		);

		DrawLine3D(
			DxConverter::VecToDx(_game->GetObjectServer()->GetPlayer()->GetPos()),
			DxConverter::VecToDx(_pos + (*iter)->GetAIComponent()->GetViewLoaclaPos()),
			GetColor(255, 0, 0)
		);
	}
}

void ModeDebugMenu::RenderEnemyAIName() {

	for (auto iter = _debugAIs.begin(); iter != _debugAIs.end(); ++iter) {
		auto _AI = (*iter);
		if (_AI) {
			VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx((*iter)->GetOwner()->GetPos() + Vector3(0, 180, 0)));
			DrawFormatString((int)pos.x, (int)pos.y, GetColor(0, 255, 0), "%s ", _AI->GetCurrentState()->GetName());
		}
	}
}

void ModeDebugMenu::RenderNavigationPolygons() {
	auto handle = _game->GetObjectServer()->GetNavi()->GetHandle();

	if (handle <= 0) { return; }

	std::vector<Polygon3D>polygons = _game->GetObjectServer()->GetNavi()->GetPolygon();

	for (auto iter = polygons.begin(); iter != polygons.end(); ++iter) {
	
		DrawTriangle3D(
			DxConverter::VecToDx((*iter).ver1),
			DxConverter::VecToDx((*iter).ver2),
			DxConverter::VecToDx((*iter).ver3),
			GetColor(255, 255, 0),
			FALSE
		);
	}
}

void ModeDebugMenu::RenerSirenInfo() {
	auto sound = GetGame()->GetObjectServer()->GetSirens();

	for (int a = 0; a < sound.size(); a++) {
	
		VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx(sound[a]->GetPos()));

		DrawFormatString((int)pos.x,(int)pos.y,GetColor(255,0,0),"残りインターバル %d",sound[a]->GetInterval());
	}
}

void ModeDebugMenu::RenderSoundInfo() {
	auto sound = GetGame()->GetObjectServer()->GetPhysWorld()->GetSoundComponent();

	for (int a = 0; a < sound.size(); a++) {
		DrawSphere3D(
		DxConverter::VecToDx(sound[a]->GetPos()),
			sound[a]->GetVolumeSize(),
			10,
			0,
			0,
			FALSE
		);
	}
}

void ModeDebugMenu::RenderDetectionInfo() {
	auto objServer = _game->GetObjectServer();

	for (auto iter = objServer->GetCommonSoldiers().begin(); iter != objServer->GetCommonSoldiers().end(); ++iter) {
		VECTOR pos = ConvWorldPosToScreenPos((*iter)->GetDxPos());

		DrawFormatStringF(pos.x, pos.y + 20.f, GetColor(255, 0, 0), "検知度 %f%", (*iter)->GetDetectionLevel());
	}
}