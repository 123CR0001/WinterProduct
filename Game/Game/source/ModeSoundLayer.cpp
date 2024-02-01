
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
	//gGlobal._sndServer.Add("BGM_FANTASY_01", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy01.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_02", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy02.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_03", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy03.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_04", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy04.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_05", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy05.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_06", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy06.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_07", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy07.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_08", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy08.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_09", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy09.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_10", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy10.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_11", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy11.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_12", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy12.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_13", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy13.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_14", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy14.mp3"));
	//gGlobal._sndServer.Add("BGM_FANTASY_15", new SoundItemBGM("res/bgm/maou_bgm_fantasy/maou_bgm_fantasy15.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_01", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic01.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_02", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic02.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_03", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic03.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_04", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic04.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_05", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic05.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_06", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic06.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_07", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic07.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_08", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic08.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_09", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic09.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_10", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic10.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_11", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic11.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_12", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic12.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_13", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic13.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_14", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic14.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_15", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic15.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_16", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic16.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_17", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic17.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_18", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic18.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_19", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic19.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_20", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic20.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_21", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic21.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_22", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic22.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_23", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic23.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_24", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic24.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_25", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic25.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_26", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic26.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_27", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic27.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_28", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic28.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_29", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic29.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_30", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic30.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_31", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic31.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_32", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic32.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_33", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic33.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_34", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic34.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_35", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic35.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_36", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic36.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_37", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic37.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_38", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic38.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_39", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic39.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_40", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic40.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_41", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic41.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_42", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic42.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_43", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic43.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_44", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic44.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_45", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic45.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_46", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic46.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_47", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic47.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_48", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic48.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_49", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic49.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_50", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic50.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_51", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic51.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_52", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic52.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_53", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic53.mp3"));
	//gGlobal._sndServer.Add("BGM_ACOUSTIC_54", new SoundItemBGM("res/bgm/maou_bgm_acoustic/maou_bgm_acoustic54.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit01", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit01.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit02", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit02.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit03", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit03.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit04", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit04.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit05", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit05.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit06", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit06.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit07", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit07.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit08", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit08.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit09", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit09.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit10", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit10.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit11", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit11.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit12", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit12.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit13", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit13.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit14", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit14.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit15", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit15.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit16", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit16.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit17", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit17.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit18", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit18.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit19", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit19.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit20", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit20.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit21", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit21.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit22", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit22.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit23", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit23.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit24", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit24.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit25", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit25.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit26", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit26.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit27", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit27.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit28", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit28.mp3"));
	//gGlobal._sndServer.Add("BGM_8bit29", new SoundItemBGM("res/bgm/maou_bgm_8bit/maou_bgm_8bit29.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber01", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber01.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber02", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber02.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber03", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber03.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber04", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber04.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber05", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber05.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber06", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber06.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber07", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber07.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber07b", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber07b.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber08", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber08.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber09", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber09.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber10", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber10.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber11", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber11.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber12", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber12.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber13", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber13.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber14", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber14.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber15", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber15.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber16", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber16.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber17", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber17.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber18", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber18.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber19", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber19.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber20", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber20.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber21", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber21.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber22", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber22.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber23", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber23.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber24", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber24.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber25", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber25.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber26", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber26.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber27", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber27.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber28", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber28.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber29", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber29.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber30", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber30.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber31", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber31.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber32", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber32.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber33", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber33.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber34", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber34.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber35", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber35.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber36", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber36.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber37", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber37.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber38", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber38.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber39", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber39.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber40", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber40.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber41", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber41.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber42", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber42.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber43", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber43.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber44", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber44.mp3"));
	//gGlobal._sndServer.Add("BGM_cyber45", new SoundItemBGM("res/bgm/maou_bgm_cyber/maou_bgm_cyber45.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic01", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic01.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic02", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic02.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic03", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic03.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic04", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic04.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic05", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic05.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic06", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic06.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic07", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic07.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic08", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic08.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic09", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic09.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic09b", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic09b.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic10", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic10.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic11", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic11.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic12", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic12.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic13", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic13.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic14", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic14.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic15", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic15.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic16", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic16.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic17", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic17.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic18", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic18.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic19", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic19.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic20", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic20.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic21", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic21.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic22", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic22.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic23", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic23.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic24", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic24.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic25", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic25.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic26", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic26.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic27", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic27.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic28", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic28.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic29", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic29.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic30", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic30.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic31", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic31.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic32", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic32.mp3"));
	//gGlobal._sndServer.Add("BGM_ethnic33", new SoundItemBGM("res/bgm/maou_bgm_ethnic/maou_bgm_ethnic33.mp3"));
	//gGlobal._sndServer.Add("BGM_healing01", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing01.mp3"));
	//gGlobal._sndServer.Add("BGM_healing02", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing02.mp3"));
	//gGlobal._sndServer.Add("BGM_healing03", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing03.mp3"));
	//gGlobal._sndServer.Add("BGM_healing04", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing04.mp3"));
	//gGlobal._sndServer.Add("BGM_healing05", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing05.mp3"));
	//gGlobal._sndServer.Add("BGM_healing06", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing06.mp3"));
	//gGlobal._sndServer.Add("BGM_healing07", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing07.mp3"));
	//gGlobal._sndServer.Add("BGM_healing08", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing08.mp3"));
	//gGlobal._sndServer.Add("BGM_healing09", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing09.mp3"));
	//gGlobal._sndServer.Add("BGM_healing10", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing10.mp3"));
	//gGlobal._sndServer.Add("BGM_healing11", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing11.mp3"));
	//gGlobal._sndServer.Add("BGM_healing11b", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing11b.mp3"));
	//gGlobal._sndServer.Add("BGM_healing12", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing12.mp3"));
	//gGlobal._sndServer.Add("BGM_healing12b", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing12b.mp3"));
	//gGlobal._sndServer.Add("BGM_healing13", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing13.mp3"));
	//gGlobal._sndServer.Add("BGM_healing14", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing14.mp3"));
	//gGlobal._sndServer.Add("BGM_healing14b", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing14b.mp3"));
	//gGlobal._sndServer.Add("BGM_healing15", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing15.mp3"));
	//gGlobal._sndServer.Add("BGM_healing16", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing16.mp3"));
	//gGlobal._sndServer.Add("BGM_healing17", new SoundItemBGM("res/bgm/maou_bgm_healing/maou_bgm_healing17.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock01", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock01.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock02", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock02.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock03", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock03.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock04", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock04.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock05", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock05.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock06", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock06.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock07", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock07.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock08", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock08.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock09", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock09.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock10", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock10.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock11", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock11.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock12", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock12.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock13", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock13.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock14", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock14.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock15", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock15.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock16", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock16.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock17", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock17.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock18", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock18.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock19", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock19.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock20", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock20.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock21", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock21.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock22", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock22.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock23", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock23.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock24", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock24.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock25", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock25.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock26", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock26.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock27", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock27.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock28", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock28.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock29", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock29.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock30", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock30.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock31", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock31.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock32", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock32.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock33", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock33.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock34", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock34.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock35", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock35.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock36", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock36.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock37", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock37.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock38", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock38.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock39", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock39.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock40", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock40.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock41", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock41.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock42", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock42.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock43", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock43.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock44", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock44.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock45", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock45.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock46", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock46.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock47", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock47.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock48", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock48.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock49", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock49.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock50", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock50.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock51", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock51.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock52", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock52.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock53", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock53.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock54", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock54.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock55", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock55.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock56", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock56.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock57", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock57.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock58", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock58.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock58_novoice", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock58_novoice.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock59", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock59.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock60", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock60.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock61", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock61.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock62", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock62.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock63", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock63.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock64", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock64.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock65", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock65.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock66", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock66.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock67", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock67.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock68", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock68.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock69", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock69.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock70", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock70.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock71", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock71.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock71b", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock71b.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock72", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock72.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock73", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock73.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock74", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock74.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock75", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock75.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock76", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock76.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock77", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock77.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock78", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock78.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock79", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock79.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock80", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock80.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock81", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock81.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock82", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock82.mp3"));
	//gGlobal._sndServer.Add("BGM_neorock83", new SoundItemBGM("res/bgm/maou_bgm_neorock/maou_bgm_neorock83.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra01", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra01.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra02", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra02.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra03", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra03.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra04", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra04.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra05", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra05.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra06", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra06.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra07", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra07.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra08", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra08.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra09", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra09.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra10", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra10.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra11", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra11.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra12", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra12.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra13", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra13.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra14", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra14.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra15", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra15.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra16", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra16.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra17", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra17.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra18", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra18.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra19", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra19.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra20", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra20.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra21", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra21.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra22", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra22.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra23", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra23.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra24", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra24.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra25", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra25.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra26", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra26.mp3"));
	//gGlobal._sndServer.Add("BGM_orchestra_sinfonia01", new SoundItemBGM("res/bgm/maou_bgm_orchestra/maou_bgm_orchestra_sinfonia01.mp3"));
	//gGlobal._sndServer.Add("BGM_piano01", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano01.mp3"));
	//gGlobal._sndServer.Add("BGM_piano02", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano02.mp3"));
	//gGlobal._sndServer.Add("BGM_piano03", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano03.mp3"));
	//gGlobal._sndServer.Add("BGM_piano04", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano04.mp3"));
	//gGlobal._sndServer.Add("BGM_piano05", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano05.mp3"));
	//gGlobal._sndServer.Add("BGM_piano06", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano06.mp3"));
	//gGlobal._sndServer.Add("BGM_piano07", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano07.mp3"));
	//gGlobal._sndServer.Add("BGM_piano08", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano08.mp3"));
	//gGlobal._sndServer.Add("BGM_piano09", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano09.mp3"));
	//gGlobal._sndServer.Add("BGM_piano10", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano10.mp3"));
	//gGlobal._sndServer.Add("BGM_piano11", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano11.mp3"));
	//gGlobal._sndServer.Add("BGM_piano12", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano12.mp3"));
	//gGlobal._sndServer.Add("BGM_piano13", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano13.mp3"));
	//gGlobal._sndServer.Add("BGM_piano14", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano14.mp3"));
	//gGlobal._sndServer.Add("BGM_piano15", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano15.mp3"));
	//gGlobal._sndServer.Add("BGM_piano16", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano16.mp3"));
	//gGlobal._sndServer.Add("BGM_piano17", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano17.mp3"));
	//gGlobal._sndServer.Add("BGM_piano18", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano18.mp3"));
	//gGlobal._sndServer.Add("BGM_piano19", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano19.mp3"));
	//gGlobal._sndServer.Add("BGM_piano20", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano20.mp3"));
	//gGlobal._sndServer.Add("BGM_piano21", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano21.mp3"));
	//gGlobal._sndServer.Add("BGM_piano22", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano22.mp3"));
	//gGlobal._sndServer.Add("BGM_piano23", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano23.mp3"));
	//gGlobal._sndServer.Add("BGM_piano24", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano24.mp3"));
	//gGlobal._sndServer.Add("BGM_piano25", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano25.mp3"));
	//gGlobal._sndServer.Add("BGM_piano26", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano26.mp3"));
	//gGlobal._sndServer.Add("BGM_piano27", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano27.mp3"));
	//gGlobal._sndServer.Add("BGM_piano28", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano28.mp3"));
	//gGlobal._sndServer.Add("BGM_piano29", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano29.mp3"));
	//gGlobal._sndServer.Add("BGM_piano30", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano30.mp3"));
	//gGlobal._sndServer.Add("BGM_piano31", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano31.mp3"));
	//gGlobal._sndServer.Add("BGM_piano32", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano32.mp3"));
	//gGlobal._sndServer.Add("BGM_piano33", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano33.mp3"));
	//gGlobal._sndServer.Add("BGM_piano34", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano34.mp3"));
	//gGlobal._sndServer.Add("BGM_piano35", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano35.mp3"));
	//gGlobal._sndServer.Add("BGM_piano36", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano36.mp3"));
	//gGlobal._sndServer.Add("BGM_piano37", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano37.mp3"));
	//gGlobal._sndServer.Add("BGM_piano38", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano38.mp3"));
	//gGlobal._sndServer.Add("BGM_piano39", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano39.mp3"));
	//gGlobal._sndServer.Add("BGM_piano40", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano40.mp3"));
	//gGlobal._sndServer.Add("BGM_piano41", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano41.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_sinfonia01", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_sinfonia01.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_sinfonia02", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_sinfonia02.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_song_ahurera", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_song_ahurera.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_song_calendula", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_song_calendula.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_song_feels_happiness", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_song_feels_happiness.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_song_milkeyway", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_song_milkeyway.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_song_noapusa", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_song_noapusa.mp3"));
	//gGlobal._sndServer.Add("BGM_piano_song_shirogane_no_kobune", new SoundItemBGM("res/bgm/maou_bgm_piano/maou_bgm_piano_song_shirogane_no_kobune.mp3"));

	gGlobal._sndServer.Add("SE_01", new SoundItemSE("res/se/tm2_bom000.wav"));
	gGlobal._sndServer.Add("SE_02", new SoundItemSE("res/se/tm2_bom001.wav"));
	gGlobal._sndServer.Add("SE_03", new SoundItemSE("res/se/tm2_bom002.wav"));
	gGlobal._sndServer.Add("SE_04", new SoundItemSE("res/se/tm2_bom003.wav"));
	gGlobal._sndServer.Add("SE_05", new SoundItemSE("res/se/tm2_bom004.wav"));
	gGlobal._sndServer.Add("SE_06", new SoundItemSE("res/se/tm2_bom005.wav"));
	gGlobal._sndServer.Add("SE_07", new SoundItemSE("res/se/tm2_bone000.wav"));
	gGlobal._sndServer.Add("SE_08", new SoundItemSE("res/se/tm2_bush001.wav"));
	gGlobal._sndServer.Add("SE_09", new SoundItemSE("res/se/tm2_coin000.wav"));
	gGlobal._sndServer.Add("SE_10", new SoundItemSE("res/se/tm2_death000.wav"));
	gGlobal._sndServer.Add("SE_11", new SoundItemSE("res/se/tm2_death001.wav"));
	gGlobal._sndServer.Add("SE_12", new SoundItemSE("res/se/tm2_heartbeat000.wav"));
	gGlobal._sndServer.Add("SE_13", new SoundItemSE("res/se/tm2_laser000.wav"));
	gGlobal._sndServer.Add("SE_14", new SoundItemSE("res/se/tm2_laser001.wav"));
	gGlobal._sndServer.Add("SE_15", new SoundItemSE("res/se/tm2_mind000.wav"));
	gGlobal._sndServer.Add("SE_16", new SoundItemSE("res/se/tm2_noise000.wav"));
	gGlobal._sndServer.Add("SE_17", new SoundItemSE("res/se/tm2_pon000.wav"));
	gGlobal._sndServer.Add("SE_18", new SoundItemSE("res/se/tm2_power000.wav"));
	gGlobal._sndServer.Add("SE_19", new SoundItemSE("res/se/tm2_power001.wav"));
	gGlobal._sndServer.Add("SE_20", new SoundItemSE("res/se/tm2_scratch000.wav"));
	gGlobal._sndServer.Add("SE_21", new SoundItemSE("res/se/tm2_shake000.wav"));
	gGlobal._sndServer.Add("SE_22", new SoundItemSE("res/se/tm2_shake001.wav"));
	gGlobal._sndServer.Add("SE_23", new SoundItemSE("res/se/tm2_shoot000.wav"));
	gGlobal._sndServer.Add("SE_24", new SoundItemSE("res/se/tm2_shoot001.wav"));
	gGlobal._sndServer.Add("SE_25", new SoundItemSE("res/se/tm2_shoot002.wav"));
	gGlobal._sndServer.Add("SE_26", new SoundItemSE("res/se/tm2_shoot003.wav"));
	gGlobal._sndServer.Add("SE_27", new SoundItemSE("res/se/tm2_slash000.wav"));
	gGlobal._sndServer.Add("SE_28", new SoundItemSE("res/se/tm2_sonic001.wav"));
	gGlobal._sndServer.Add("SE_29", new SoundItemSE("res/se/tm2_sonic002.wav"));
	gGlobal._sndServer.Add("SE_30", new SoundItemSE("res/se/tm2_sonic003.wav"));
	gGlobal._sndServer.Add("SE_31", new SoundItemSE("res/se/tm2_sonic004.wav"));
	gGlobal._sndServer.Add("SE_32", new SoundItemSE("res/se/tm2_sonic005.wav"));

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

