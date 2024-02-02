#include"ModeDebugMenu.h"
#include "ApplicationMain.h"
#include"CommonSoldier.h"
#include"AIBackPatrol.h"
#include"AIChasePlayer.h"
#include"AIComponent.h"
#include"AICheckPoint.h"
#include"AIPatrol.h"
#include"Player.h"
#include"ObjectServer.h"
#include"CapsuleComponent.h"

#include"ModeEffekseer.h"
#include"ModeGame.h"

bool ModeDebugMenu::_charaCollision = false;
bool ModeDebugMenu::_root = false;
bool ModeDebugMenu::_visionCollision = false;
bool ModeDebugMenu::_AIName = false;
bool ModeDebugMenu::_cameraChange = false;
bool ModeDebugMenu::_navigationPolygon = false;

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
	const char* GetText()override { return "カメラの処理を切り替えます"; }
	void Select()override {
		ModeDebugMenu::_cameraChange = !ModeDebugMenu::_cameraChange;
		if (ModeDebugMenu::_cameraChange) {
			auto player = _menu->GetGame()->GetObjectServer()->GetPlayer();
			playerCamera = player->GetCamera();
			camera = new OrbitCamera(player, 998);
			player->SetCamera(camera);
		}
		else {
			auto player = _menu->GetGame()->GetObjectServer()->GetPlayer();
			player->GetCamera()->Initialize();
			player->SetCamera(playerCamera);
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
	const char* GetText()override { return "ナビゲーション用のポリゴンを描画します"; }
	void Select()override {
		ModeDebugMenu::_navigationPolygon = !ModeDebugMenu::_navigationPolygon;
	}
};

class GameEnd :public ModeDebugMenu::DebugMenuItem {
public:
	GameEnd(ModeDebugMenu* menu) :DebugMenuItem(menu) {}
	const char* GetText()override { return "ゲームを終了します"; }
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
			player->GetPos()+Vector3D(0.f,180.f,0.f), 
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
	const char* GetText()override { return "キャラやステージの状態を初期化します"; }
	void Select()override {
		_menu->GetGame()->GetObjectServer()->Terminate();
		_menu->GetGame()->LoadData();
		_menu->GetGame()->GetObjectServer()->ProcessInit();
	}
};

//ModeDebugMenu::ModeDebugMenu(ModeGame* game)
//	:_game(game) {
//
//}

ModeDebugMenu::~ModeDebugMenu(){

}

bool ModeDebugMenu::Initialize() {
	_selectNum = 0;
	_isProcess = false;
	_inputFrameCnt = 0;
	_start = 0;

	_debugMenus.emplace_back(new CharaCollision(this));
	_debugMenus.emplace_back(new CommonSoldierRoot(this));
	_debugMenus.emplace_back(new CommonSolidierVisionCollision(this));
	_debugMenus.emplace_back(new CommonSoldierAIName(this));
	_debugMenus.emplace_back(new PlayerCameraChange(this));
	_debugMenus.emplace_back(new NavigationPolygons(this));
	_debugMenus.emplace_back(new GameEnd(this));
	_debugMenus.emplace_back(new StageReset(this));
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

		// このモードより下のレイヤーはProcess()を呼ばない
		ModeServer::GetInstance()->SkipProcessUnderLayer();
	}

	if (trg & INPUT_BACK) {
		_isProcess = !_isProcess;
	}


	return true;
}

bool ModeDebugMenu::Render() {

	if (_charaCollision) { RenderCharaCollision(); }
	if (_root) { RenderEnemyRoot(); }
	if (_visionCollision) { RenderVisionCollision(); }
	if (_AIName) { RenderEnemyAIName(); }
	if (_navigationPolygon) { RenderNavigationPolygons(); }

	if (_isProcess) {
		SetFontSize(FONT_SIZE);

		DrawBox(0, 0, 500, MENU_HEIGHT, GetColor(255, 0, 0), TRUE);

		if (_selectNum - _start >= FITS_MENU_UI_NUM) { _start++; }
		if (_selectNum < _start) { _start--; }

		DrawFormatString(FONT_SIZE,( _selectNum - _start)* FONT_SIZE, GetColor(0, 0, 255), "→");

		for (int a = _start; a < _start + _drawNum; a++) {
			auto pos = Vector3D(50.f, (float)FONT_SIZE * (a - _start), 0.f);
			DrawFormatString((int)pos.x, (int)pos.y, GetColor(0, 0, 255), "%s", _debugMenus[a]->GetText());
		}
		SetFontSize(GetFontSize());
	}

	return true;
}

void ModeDebugMenu::RenderCharaCollision() {
	auto charas = _game->GetObjectServer()->GetCharas();

	if (charas.size() == 0) { return; }

	for (auto iter = charas.begin(); iter != charas.end(); ++iter) {
		auto _pos = (*iter)->GetPos();
		auto _colSubY = (*iter)->GetCollSubY();
		auto _radian = (*iter)->GetRadian();

		DrawCapsule3D(
			VAdd(DxConverter::VecToDx(_pos), VGet(0, _colSubY + _radian, 0)),
			VAdd(DxConverter::VecToDx(_pos), VGet(0, _radian, 0)),
			_radian, 10,
			GetColor(255, 0, 0),
			GetColor(255, 255, 255),
			FALSE
		);

		DrawLine3D(
			DxConverter::VecToDx(_pos + Vector3D(0, _colSubY + _radian * 2, 0)),
			DxConverter::VecToDx(_pos + Vector3D(0, -1, 0)),
			GetColor(0, 0, 255)
		);
		DrawSphere3D(
			DxConverter::VecToDx(_pos),
			_radian,
			10,
			GetColor(0, 255, 0),
			GetColor(255, 255, 255),
			FALSE
		);

	}
}

void ModeDebugMenu::RenderEnemyRoot() {

	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();

	if (CommonSoldiers.size() == 0) { return; }


	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {

		VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx((*iter)->GetPos() + Vector3D(0, 180, 0)));

		auto AI = (*iter)->GetAIComponent();

		if (!AI) { continue; }

		int num = 0;

		for (auto route : AI->GetPoints(AI->GetCurrentState()->GetName())) {
			num++;
			DrawSphere3D(
				DxConverter::VecToDx(route + Vector3D(0, 30, 0)),
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
			DrawFormatString((int)pos.x, (int)pos.y, GetColor(255, 255, 255), "最大サイズ%d", AI->GetPoints(AI->GetCurrentState()->GetName()));
		}

	}
	/*	auto _pos = (*iter)->GetPos();
		VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx(_pos + Vector3D(0, 180, 0)));

		if (_AI->GetCurrentState()->GetName() == "Patrol") {

			AIPatrol* state = static_cast<AIPatrol*>(_AI->GetCurrentState());
			if (state->GetPatrolPoints().size() > state->GetPatrolPointsNum()) {
				DrawLine3D(
					DxConverter::VecToDx(state->GetPatrolPoints()[state->GetPatrolPointsNum()] + Vector3D(0, 100, 0)),
					DxConverter::VecToDx(_pos + Vector3D(0, 100, 0)),
					GetColor(0, 255, 0)
				);

				for (int a = 0; a < state->GetPatrolPoints().size(); a++) {
					DrawSphere3D(
						DxConverter::VecToDx(state->GetPatrolPoints()[a] + Vector3D(0, 30, 0)),
						30,
						10,
						GetColor(0, 255, 0),
						GetColor(255, 255, 255),
						FALSE
					);
					DrawFormatString(
						ConvWorldPosToScreenPos(DxConverter::VecToDx(state->GetPatrolPoints()[a])).x,
						ConvWorldPosToScreenPos(DxConverter::VecToDx(state->GetPatrolPoints()[a])).y,
						GetColor(255, 255, 255), 
						"%d番目 x = %3f y = %3f z = %3f", 
						a, state->GetPatrolPoints()[a].x, state->GetPatrolPoints()[a].y, state->GetPatrolPoints()[a].z
					);
				}
			}
			DrawFormatString(pos.x, pos.y, GetColor(255, 255, 255), "最大サイズ%d %d番目", state->GetPatrolPoints().size(), state->GetPatrolPointsNum());
		}
		else if (_AI->GetCurrentState()->GetName() == "CheckPlayer") {
			AIChasePlayer* state = dynamic_cast<AIChasePlayer*>(_AI->GetCurrentState());
			if (state->GetPoints().size() > state->GetPointsNum()) {
				DrawLine3D(
					DxConverter::VecToDx(state->GetPoints()[state->GetPointsNum()] + Vector3D(0, 100, 0)),
					DxConverter::VecToDx(_pos + Vector3D(0, 100, 0)),
					GetColor(0, 255, 0)
				);

				for (int a = 0; a < state->GetPoints().size(); a++) {
					DrawSphere3D(
						DxConverter::VecToDx(state->GetPoints()[a] + Vector3D(0, 30, 0)),
						30,
						10,
						GetColor(0, 255, 0),
						GetColor(255, 255, 255),
						FALSE
					);

				}
			}
			DrawFormatString(pos.x, pos.y, GetColor(255, 255, 255), "最大サイズ%d %d番目", state->GetPoints().size(), state->GetPointsNum());
		}
		else if (_AI->GetCurrentState()->GetName() == "BackPatrol") {
			AIBackPatrol* state = dynamic_cast<AIBackPatrol*>(_AI->GetCurrentState());
			if (state->GetPoints().size() > state->GetPointsNum()) {
				DrawLine3D(
					DxConverter::VecToDx(state->GetPoints()[state->GetPointsNum()] + Vector3D(0, 100, 0)),
					DxConverter::VecToDx(_pos + Vector3D(0, 100, 0)),
					GetColor(0, 255, 0)
				);

				for (int a = 0; a < state->GetPoints().size(); a++) {
					DrawSphere3D(
						DxConverter::VecToDx(state->GetPoints()[a] + Vector3D(0, 30, 0)),
						30,
						10,
						GetColor(0, 255, 0),
						GetColor(255, 255, 255),
						FALSE
					);

				}
			}
			DrawFormatString(pos.x, pos.y, GetColor(255, 255, 255), "最大サイズ%d %d番目", state->GetPoints().size(), state->GetPointsNum());
		}
	}*/
}

void ModeDebugMenu::RenderVisionCollision() {
	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();

	if (CommonSoldiers.size() == 0) { return; }

	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {
		auto _pos = (*iter)->GetPos();
		auto _eulerAngle = (*iter)->GetEulerAngle();
		auto _visionAngle = (*iter)->GetVisionAngle();
		auto _visionDist = (*iter)->GetVisionDist();

		int color = GetColor(255, 0, 0);


		if ((*iter)->IsPlayerFound()) {
			color = GetColor(0, 0, 255);
		}

		float angle = _eulerAngle.y + DegToRad(_visionAngle / 2);

		Vector3D vDir(0, 0, 0);

		vDir.x = sin(angle) * _visionDist;
		vDir.z = cos(angle) * _visionDist;

		Vector3D right(vDir + _pos);

		angle = _eulerAngle.y - DegToRad(_visionAngle / 2);

		vDir.x = sin(angle) * _visionDist;
		vDir.z = cos(angle) * _visionDist;

		Vector3D left(vDir + _pos);

		vDir.x = sin(_eulerAngle.y) * _visionDist;
		vDir.z = cos(_eulerAngle.y) * _visionDist;

		Vector3D forward(_pos + vDir);

		DrawTriangle3D(
			DxConverter::VecToDx(_pos + Vector3D(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(forward + Vector3D(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(right + Vector3D(0.f, 0.1f, 0.f)),
			color,
			TRUE
		);
		DrawTriangle3D(
			DxConverter::VecToDx(_pos + Vector3D(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(left + Vector3D(0.f, 0.1f, 0.f)),
			DxConverter::VecToDx(forward + Vector3D(0.f, 0.1f, 0.f)),
			color,
			TRUE
		);

		DrawSphere3D(
			DxConverter::VecToDx(_pos + Vector3D(0, 100, 0)),
			30,
			10,
			color,
			GetColor(255, 255, 255),
			FALSE
		);

		DrawLine3D(
			DxConverter::VecToDx(Collision::SegPointLatestPoint(Vector3D(_pos + Vector3D(0, 100, 0)), _game->GetObjectServer()->GetPlayer()->GetCapsuleSegment())),
			DxConverter::VecToDx(_pos + Vector3D(0, 100, 0)),
			GetColor(255, 0, 0)
		);
	}
}

void ModeDebugMenu::RenderEnemyAIName() {
	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();

	if (CommonSoldiers.size() == 0) { return; }

	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {
		auto _AI = (*iter)->GetAIComponent();
		if (_AI) {
			VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx((*iter)->GetPos() + Vector3D(0, 180, 0)));
			DrawFormatString((int)pos.x, (int)pos.y, GetColor(0, 255, 0), "%s ", _AI->GetCurrentState()->GetName());
		}
	}
}

void ModeDebugMenu::RenderNavigationPolygons() {
	auto handle = _game->GetObjectServer()->GetNavigationHandle();

	if (handle <= 0) { return; }

	std::vector<Polygon3D>polygons;

	Navi::GetPolygonData(handle, &polygons);

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