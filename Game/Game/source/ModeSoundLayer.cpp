
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeSoundLayer.h"


bool ModeSoundLayer::Initialize() {
	if (!base::Initialize()) { return false; }

	// 非同期読み込み設定
	SetUseASyncLoadFlag(TRUE);

	int timems = GetNowCount();
	_asyncLoadBaseTimeMs = timems;	// 非同期ロード開始時間
	_asyncLoadTimeMs = 0;

	//// 音データファイルを読み込む
	gGlobal._sndServer.Add("BGM_01", new SoundItemBGM("res/Sound/bgm/title_01.wav"));
	gGlobal._sndServer.Add("BGM_02", new SoundItemBGM("res/Sound/bgm/stageselect_01.wav"));
	gGlobal._sndServer.Add("BGM_03", new SoundItemBGM("res/Sound/bgm/mainstage_01.wav"));
	gGlobal._sndServer.Add("BGM_04", new SoundItemBGM("res/Sound/bgm/mainstage_02.wav"));
	gGlobal._sndServer.Add("BGM_05", new SoundItemBGM("res/Sound/bgm/mainstage_03.wav"));
	gGlobal._sndServer.Add("BGM_06", new SoundItemBGM("res/Sound/bgm/lightsout_01.wav"));
	gGlobal._sndServer.Add("BGM_07", new SoundItemBGM("res/Sound/bgm/gameover_01.wav"));
	gGlobal._sndServer.Add("BGM_08", new SoundItemBGM("res/Sound/bgm/result_01.wav"));


	gGlobal._sndServer.Add("SE_01", new SoundItemSE("res/Sound/se/footsteps_01.wav"));
	gGlobal._sndServer.Add("SE_02", new SoundItemSE("res/Sound/se/knifeattack_01.wav"));
	gGlobal._sndServer.Add("SE_03", new SoundItemSE("res/Sound/se/knifehit_01.wav"));
	gGlobal._sndServer.Add("SE_04", new SoundItemSE("res/Sound/se/falldown_01.wav"));
	gGlobal._sndServer.Add("SE_05", new SoundItemSE("res/Sound/se/detectionrise_01.wav"));
	gGlobal._sndServer.Add("SE_06", new SoundItemSE("res/Sound/se/gunshot_01.wav"));
	gGlobal._sndServer.Add("SE_07", new SoundItemSE("res/Sound/se/siren_01.wav"));
	gGlobal._sndServer.Add("SE_08", new SoundItemSE("res/Sound/se/decoy_01.wav"));
	gGlobal._sndServer.Add("SE_09", new SoundItemSE("res/Sound/se/lightsoutstart_01.wav"));
	gGlobal._sndServer.Add("SE_10", new SoundItemSE("res/Sound/se/lightsoutfinish_01.wav"));
	gGlobal._sndServer.Add("SE_11", new SoundItemSE("res/Sound/se/selection_01.wav"));
	gGlobal._sndServer.Add("SE_12", new SoundItemSE("res/Sound/se/decision_01.wav"));
	gGlobal._sndServer.Add("SE_13", new SoundItemSE("res/Sound/se/confirmation_01.wav"));
	gGlobal._sndServer.Add("SE_14", new SoundItemSE("res/Sound/se/pause_01.wav"));
	gGlobal._sndServer.Add("SE_15", new SoundItemSE("res/Sound/se/timer_01.wav"));

	gGlobal._sndServer.Add("SE_16", new SoundItemSE("res/Sound/se/a_reaction_01.wav"));
	gGlobal._sndServer.Add("SE_17", new SoundItemSE("res/Sound/se/a_reaction_02.wav"));
	gGlobal._sndServer.Add("SE_18", new SoundItemSE("res/Sound/se/b_reaction_01.wav"));
	gGlobal._sndServer.Add("SE_19", new SoundItemSE("res/Sound/se/b_reaction_02.wav"));
	gGlobal._sndServer.Add("SE_20", new SoundItemSE("res/Sound/se/c_reaction_01.wav"));

	gGlobal._sndServer.Add("SE_21", new SoundItemSE("res/Sound/se/a_miss_01.wav"));
	gGlobal._sndServer.Add("SE_22", new SoundItemSE("res/Sound/se/a_miss_02.wav"));
	gGlobal._sndServer.Add("SE_23", new SoundItemSE("res/Sound/se/b_miss_01.wav"));
	gGlobal._sndServer.Add("SE_24", new SoundItemSE("res/Sound/se/b_miss_02.wav"));
	gGlobal._sndServer.Add("SE_25", new SoundItemSE("res/Sound/se/c_miss_01.wav"));

	gGlobal._sndServer.Add("SE_26", new SoundItemSE("res/Sound/se/enemyfootstep_01.wav"));
	gGlobal._sndServer.Add("SE_27", new SoundItemSE("res/Sound/se/owldeath_01.wav"));

	gGlobal._sndServer.Add("SE_28", new SoundItemSE("res/Sound/se/energy_01.wav"));

	gGlobal._sndServer.Add("SE_29", new SoundItemSE("res/Sound/se/owlclear_01.wav"));

	//gGlobal._sndServer.Add("VOICE_01", new SoundItemVOICE("res/voice/「あとちょっとだったね」.mp3"));
	//gGlobal._sndServer.Add("VOICE_02", new SoundItemVOICE("res/voice/「おめでとう」.mp3"));
	//gGlobal._sndServer.Add("VOICE_03", new SoundItemVOICE("res/voice/「おめでとうございます」.mp3"));
	//gGlobal._sndServer.Add("VOICE_04", new SoundItemVOICE("res/voice/「がんばりましょう」.mp3"));
	//gGlobal._sndServer.Add("VOICE_05", new SoundItemVOICE("res/voice/「すごいすごい」.mp3"));
	//gGlobal._sndServer.Add("VOICE_06", new SoundItemVOICE("res/voice/「もう一息です」.mp3"));
	//gGlobal._sndServer.Add("VOICE_07", new SoundItemVOICE("res/voice/「よくできました」.mp3"));
	//gGlobal._sndServer.Add("VOICE_08", new SoundItemVOICE("res/voice/「エクセレント」.mp3"));
	//gGlobal._sndServer.Add("VOICE_09", new SoundItemVOICE("res/voice/「グッド」.mp3"));
	//gGlobal._sndServer.Add("VOICE_10", new SoundItemVOICE("res/voice/「マーベラス」.mp3"));
	//gGlobal._sndServer.Add("VOICE_11", new SoundItemVOICE("res/voice/「不合格です」.mp3"));
	//gGlobal._sndServer.Add("VOICE_12", new SoundItemVOICE("res/voice/「合格です」.mp3"));
	//gGlobal._sndServer.Add("VOICE_13", new SoundItemVOICE("res/voice/「残念」.mp3"));
	//gGlobal._sndServer.Add("VOICE_14", new SoundItemVOICE("res/voice/「残念でした」.mp3"));
	//gGlobal._sndServer.Add("VOICE_15", new SoundItemVOICE("res/voice/「頑張ったね」.mp3"));

	// 読み込み時間
	_loadTimeMs = GetNowCount() - timems;

	// 同期読み込み設定
	SetUseASyncLoadFlag(FALSE);

	// 選択リスト用
	_listCur = 0;
	_listViewStart = 0;
	_listViewNum = 20;

	return true;
}

bool ModeSoundLayer::Terminate() {
	base::Terminate();
	return true;
}

bool ModeSoundLayer::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// サウンドサーバ更新
	gGlobal._sndServer.Update();

	// 非同期ロードは終わったか？
	if (GetASyncLoadNum() == 0) {
		if (_asyncLoadTimeMs == 0) {
			// 非同期ロードにかかった時間
			_asyncLoadTimeMs = GetNowCount() - _asyncLoadBaseTimeMs;
		}
	}
	return true;
}

//
//void ModeSoundLayer::PrintSoundItem(int x, int y, std::string name, SoundItemBase* snd) {
//	//int isLoad = 0;
//	//int isPlay = 0;
//	//int isOnMemory = 0;
//	//if (snd->IsLoad()) { isLoad = 1; }
//	//if (snd->IsPlay()) { isPlay = 1; }
//	//if (snd->GetSoundHandle() != -1) { isOnMemory = 1; }
//	//char strIsLoaded[][128] = { "(非同期ロード中)", "" };
//	//char strIsPlay[][128] = { "", "(再生中)" };
//	//char strIsOnMemory[][128] = { "", "(オンメモリ)" };
//	//DrawFormatString(32, y, GetColor(255, 255, 255),
//	//	"%s  : vol=%d, pan=%d, freq=%d,  %s  %s%s%s"
//	//	, name.c_str()
//	//	, snd->GetVolume()
//	//	, snd->GetPan()
//	//	, snd->GetFrequency()
//	//	, snd->GetFileName().c_str()
//	//	, strIsLoaded[isLoad]
//	//	, strIsPlay[isPlay]
//	//	, strIsOnMemory[isOnMemory]
//	//);
//}

bool ModeSoundLayer::Render() {
	base::Render();

	//int y = 0, size = 16;
	//DrawFormatString(0, y, GetColor(255, 0, 0), "ロード時間(ms): %d", _loadTimeMs); y += size;
	//DrawFormatString(0, y, GetColor(255, 0, 0), "非同期読み込み中ファイル数: %d", GetASyncLoadNum()); y += size;
	//DrawFormatString(0, y, GetColor(255, 0, 0), "非同期ロード時間(ms): %d", _asyncLoadTimeMs); y += size;
	//DrawFormatString(0, y, GetColor(255, 255, 255), "msg: %s", _msg.c_str()); y += size;

	//// 選択リスト＆カーソル表示
	//y = 100;
	//int n = 0;
	//for (auto&& e : gGlobal._sndServer.GetVector()) {
	//	if (n >= _listViewStart && n < _listViewStart + _listViewNum) {
	//		if (n == _listCur) {
	//			DrawFormatString(8, y, GetColor(255, 255, 255), "→");
	//		}
	//		PrintSoundItem(32, y, e.first, e.second);
	//		y += size;
	//	}
	//	n++;
	//}

	//// OneShotリスト表示
	//y += size;
	//DrawFormatString(0, y, GetColor(255, 0, 0), "--- ONESHOT ---", _loadTimeMs); y += size;
	//for (auto&& e : gGlobal._sndServer.GetVector()) {
	//	if(e.second->GetType() == SoundItemBase::TYPE::ONESHOT) {
	//		PrintSoundItem(32, y, e.first, e.second);
	//		y += size;
	//	}
	//}

	return true;
}

