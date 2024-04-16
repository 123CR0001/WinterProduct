
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeSoundLayer.h"
#include"ApplicationGlobal.h"
// 実体
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }


	// アプリケーショングローバルの初期化
	gGlobal.Init();

	// モードの登録
	//ModeServer::GetInstance()->Add(new ModeSelect(), 1, "select");
	ModeServer::GetInstance()->Add(new ModeSoundLayer(), 99999, "SoundLayer");
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");


	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	return true;
}


bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	return true;
}

bool ApplicationMain::ApplicationMain::BeforeDXLib_Init() {
	// 3DSound: XAudioを使用
	SetEnableXAudioFlag(TRUE);			// 有効にするとパンやリバーブが有効になる

	// 3DSound:１メートルに相当する値を設定する
	Set3DSoundOneMetre(100.f);

	return true;
}