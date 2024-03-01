#include "appframe.h"
#include"MotionData.h"
#include "SoundServer.h"
#include "Result.h"

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
	Result		_result;

}; 

// 他のソースでgGlobalを使えるように
extern ApplicationGlobal		gGlobal;
