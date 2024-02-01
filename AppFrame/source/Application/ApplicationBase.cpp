
#include "ApplicationBase.h"


ApplicationBase	*ApplicationBase::_lpInstance = NULL;


ApplicationBase::ApplicationBase() {
	_lpInstance = this;
	_pad = new XGamePad(DX_INPUT_PAD1);
}

ApplicationBase::~ApplicationBase() {

}

bool ApplicationBase::Initialize(HINSTANCE hInstance) {

	// DXライブラリの初期化
	if(AppWindowed()){
		ChangeWindowMode(true);							// ウィンドウモードに指定する
	}
	SetGraphMode(DispSizeW(), DispSizeH(), 32);

	if (DxLib_Init() == -1)
	{	// エラーが起きたら直ちに終了
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// 描画先画面を裏画面にセット

	// 距離が近いところでチラツキが発生する（Z深度が足りない）対策
	SetZBufferBitDepth(32);

	// Effekseerを初期化する。
// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 乱数初期化
	srand((unsigned int)time(NULL));

	// モードサーバの初期化
	_serverMode = new ModeServer();

	_isGameEnd = false;

	ResourceServer::Init();

	return true;
}

bool ApplicationBase::Terminate() {
	delete _pad;

	ResourceServer::Release();

	// Effekseerを終了する。
	Effkseer_End();

	// DXライブラリ開放
	DxLib_End();

	return true;
}


bool ApplicationBase::Input() {
	// キーの入力、トリガ入力を得る
	int keyold = _gKey;
	_gKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	_gTrg = (_gKey ^ keyold) & _gKey;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）

	_pad->Input();

	old_mouse_x = _mouse_x;
	old_mouse_y = _mouse_y;

	int old_mouse = _mouse_key;
	_mouse_key = GetMouseInput();
	_mouse_trg = (_mouse_key ^ old_mouse) & _mouse_key;
	_mouse_rel = (_mouse_key ^ old_mouse) & ~_mouse_key;

	GetMousePoint(&_mouse_x, &_mouse_y);


	return true;
}

bool ApplicationBase::Process() {
	_serverMode->ProcessInit();
	_serverMode->Process();
	_serverMode->ProcessFinish();
	return true;
}

bool ApplicationBase::Render() {
	_serverMode->RenderInit();
	_serverMode->Render();
	_serverMode->RenderFinish();
	return true;
}

