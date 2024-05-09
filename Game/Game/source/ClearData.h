#pragma once
#include<string>
	//クリア画面のリザルトデータ
class ClearData {
public:
	ClearData() {
		maxCombo = 0;
		clearSecondTime = 0.f;
		maxDetectionLevel = 0.f;
	}
	~ClearData() {}

	//最高コンボ数
	int maxCombo;

	//クリアタイム
	int clearSecondTime;

	//最高検知度
	float maxDetectionLevel;

	//実際にはリザルトには、関係のないデータ
	//今のステージ識別情報
	std::string _stageName;

	//次のステージの識別情報
	std::string _nextStageName;

	//上記のデータからランクを算出
	const char* GetRank() {
		int point = 0;
		//時間
		if (clearSecondTime < 60 * 1) {			//評価S
			point += 3;
		}
		else if (clearSecondTime < 60 * 2) {	//評価A
			point += 2;
		}
		else if (clearSecondTime < 60 * 3) {	//評価B
			point += 1;
		}
		else {										//評価C
			point += 0;
		}

		//最高検知度
		if (maxDetectionLevel < 0.25f) {				//評価S
			point += 3;
		}
		else if (maxDetectionLevel < 0.5f) {			//評価A
			point += 2;
		}
		else if (maxDetectionLevel < 0.75f) {		//評価B
			point += 1;
		}
		else {										//評価C
			point += 0;
		}

		const char* rank = nullptr;

		//上記のポイントからランクを出力
		if (point <= 1) {
			rank = "c";
		}
		else if (point <= 3) {
			rank = "b";
		}
		else if (point <= 5) {
			rank = "a";
		}
		else {
			rank = "s";
		}

		return rank;
	}
};
