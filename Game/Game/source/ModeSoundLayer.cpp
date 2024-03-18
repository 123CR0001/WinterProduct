
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
	gGlobal._sndServer.Add("SE_30", new SoundItemSE("res/Sound/se/a_death_01.wav"));
	gGlobal._sndServer.Add("SE_31", new SoundItemSE("res/Sound/se/a_death_02.wav"));
	gGlobal._sndServer.Add("SE_32", new SoundItemSE("res/Sound/se/a_death_03.wav"));
	gGlobal._sndServer.Add("SE_33", new SoundItemSE("res/Sound/se/b_death_01.wav"));
	gGlobal._sndServer.Add("SE_34", new SoundItemSE("res/Sound/se/b_death_02.wav"));
	gGlobal._sndServer.Add("SE_35", new SoundItemSE("res/Sound/se/b_death_03.wav"));
	gGlobal._sndServer.Add("SE_36", new SoundItemSE("res/Sound/se/c_death_01.wav"));
	gGlobal._sndServer.Add("SE_37", new SoundItemSE("res/Sound/se/c_death_02.wav"));
	gGlobal._sndServer.Add("SE_38", new SoundItemSE("res/Sound/se/a_impatience_01.wav"));
	gGlobal._sndServer.Add("SE_39", new SoundItemSE("res/Sound/se/a_impatience_02.wav"));
	gGlobal._sndServer.Add("SE_40", new SoundItemSE("res/Sound/se/a_impatience_03.wav"));
	gGlobal._sndServer.Add("SE_41", new SoundItemSE("res/Sound/se/b_impatience_01.wav"));
	gGlobal._sndServer.Add("SE_42", new SoundItemSE("res/Sound/se/b_impatience_02.wav"));
	gGlobal._sndServer.Add("SE_43", new SoundItemSE("res/Sound/se/c_impatience_01.wav"));
	gGlobal._sndServer.Add("SE_44", new SoundItemSE("res/Sound/se/c_impatience_02.wav"));
	gGlobal._sndServer.Add("SE_45", new SoundItemSE("res/Sound/se/c_reaction_02.wav"));

	gGlobal._sndServer.Add("SE_46", new SoundItemSE("res/Sound/se/a_discovery_01.wav"));
	gGlobal._sndServer.Add("SE_47", new SoundItemSE("res/Sound/se/a_discovery_02.wav"));
	gGlobal._sndServer.Add("SE_48", new SoundItemSE("res/Sound/se/b_discovery_01.wav"));
	gGlobal._sndServer.Add("SE_49", new SoundItemSE("res/Sound/se/c_discovery_01.wav"));


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



bool ModeSoundLayer::Render() {
	base::Render();
	return true;
}

