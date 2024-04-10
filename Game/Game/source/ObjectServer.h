#pragma once
#include<vector>
#include"appframe.h"

class ObjectServer {

public:
	ObjectServer(class ModeGame* game);
	~ObjectServer();

	void AddObject(class ObjectBase* obj);
	void DeleteObject(class ObjectBase* obj);

	bool Initialize();		//初期化
	bool Terminate();		//終了処理
	bool ProcessInit();		//処理の手前に呼び出す初期化
	bool Process();			//処理
	bool Renderer();		//描画

	//オーナー
	class ModeGame* GetGame()const { return _game; }

	//プレイヤー
	class Player* GetPlayer() { return _player; }
	void SetPlayer(class Player* set) { _player = set; }

	//オブジェクトのコンテナ
	std::vector<class ObjectBase*> GetObjects()const { return _objects; }

	//この関数を利用して、各クラスで追加されたアドレスを登録する
	std::vector<class CharaBase*>& GetCharas() { return _charas; }
	std::vector<class CommonSoldier*>& GetCommonSoldiers() { return _commonSoldiers; }
	std::vector<class Siren*>& GetSirens() { return _sirens; }

	//オブジェクトのレイアウトを読み込み、オブジェクトを生成
	bool LoadData(std::string stageName);

	//ナビクラスの取得
	class Navi* GetNavi()const { return _navi; }

	//物理クラスを取得
	class PhysWorld* GetPhysWorld()const { return _physWorld; }

	//_objects _add _delete のコンテナが保持するアドレスをdelete、各コンテナのサイズを０にする
	bool ClearObject();

private:
	//オブジェクト
	std::vector<class ObjectBase*> _objects;
	//追加するオブジェクトのアドレス
	std::vector<class ObjectBase*> _addObj;
	//削除するオブジェクトのアドレス
	std::vector<class ObjectBase*> _deleteObj;

	//特定のクラスだけ欲しいので、アドレス登録用の配列
	//用途は当たり判定やObjectBase*型のポインタから特定のクラスしかできない処理をダウンキャストせずにするため
	std::vector<class CharaBase*> _charas;		
	std::vector<class ObjectBase*>_enemys;
	std::vector<class CommonSoldier*> _commonSoldiers;
	std::vector<class Siren*>_sirens;

	//物理クラス
	class PhysWorld* _physWorld;

	//プレイヤークラス
	class Player* _player;

	//ナビクラス
	class Navi* _navi;

	//ModeGameが持つ機能や情報を利用するために、自身をnewしたModeGameクラスのアドレスと保持
	class ModeGame* _game;

};

//ステージごとに変わるパラメーター
struct STAGE_DATA_ITEM {
	int decoyTimes = 0;
	std::string nextStageName;
};

//STAGE_DATA_ITEMの読み込み用Jsonクラス
class StageDataJson :public Json {
public:
	StageDataJson(std::string fileName) {
		if (LoadData(fileName)) {
			_success = true;
			return;
		}
		_success = false;
	}
	virtual ~StageDataJson() {}

	bool LoadData(std::string file_name)override {
		if (!Json::LoadData(file_name)) { return false; }

		for (auto& data : _jsonData) {
			STAGE_DATA_ITEM item = {
				data.at("DecoyTimes"),
				data.at("NextStageName")
			};

			_stageDataItem = item;
		}

		return true;
	}

	STAGE_DATA_ITEM GetStageData()const { return _stageDataItem; }

	STAGE_DATA_ITEM _stageDataItem;
};