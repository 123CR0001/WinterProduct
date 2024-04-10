#include "appframe.h"
#include"MotionData.h"
#include "SoundServer.h"

class ApplicationGlobal
{
public:
	ApplicationGlobal();
	virtual ~ApplicationGlobal();

	//モーションデータの読み込み
	void LoadMotionData();

	bool Init();

public:
	//キャラの種類につき登録
	std::unordered_map<std::string/*キャラ名*/, MOTION_DATA> _charaMotionData;

	SoundServer	_sndServer;

}; 

// 他のソースでgGlobalを使えるように
extern ApplicationGlobal		gGlobal;
