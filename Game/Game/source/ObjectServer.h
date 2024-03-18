#pragma once
#include<vector>
#include"appframe.h"

class ObjectServer {

public:
	ObjectServer(class ModeGame* game);
	~ObjectServer();

	void AddObject(class ObjectBase* obj);
	void DeleteObject(class ObjectBase* obj);

	bool Initialize();
	bool Terminate();
	bool ProcessInit();
	bool Process();
	bool Renderer();

	class ModeGame* GetGame()const { return _game; }

	class Player* GetPlayer() { return _player; }
	void SetPlayer(class Player* set) { _player = set; }

	std::vector<class ObjectBase*> GetObjects()const { return _objects; }

	//この関数を利用して、各クラスで追加されたアドレスを登録する
	std::vector<class CharaBase*>& GetCharas() { return _charas; }
	std::vector<class ObjectBase*>& GetEnemys() { return _enemys; }
	std::vector<class CommonSoldier*>& GetCommonSoldiers() { return _commonSoldiers; }
	std::vector<class Siren*>& GetSirens() { return _sirens; }

	bool LoadData(std::string stageName);

	class Navi* GetNavi()const { return _navi; }

	class PhysWorld* GetPhysWorld()const { return _physWorld; }

	//_objects _add _delete のコンテナが保持するアドレスをdelete、サイズを０にする
	bool ClearObject();

private:
	std::vector<class ObjectBase*> _objects;
	std::vector<class ObjectBase*> _addObj;
	std::vector<class ObjectBase*> _deleteObj;

	//特定のクラスだけ欲しいので、アドレス登録用の配列
	//用途は当たり判定やObjectBase*型のポインタから特定のクラスしかできない処理をダウンキャストせずにするため
	std::vector<class CharaBase*> _charas;		
	std::vector<class ObjectBase*>_enemys;
	std::vector<class CommonSoldier*> _commonSoldiers;
	std::vector<class Siren*>_sirens;

	class PhysWorld* _physWorld;

	class Player* _player;

	class Navi* _navi;

	//ModeGameが持つ機能や情報を利用するために、自身をnewしたModeGameクラスのアドレスと保持
	class ModeGame* _game;

};

struct STAGE_DATA_ITEM {
	int decoyTimes = 0;
};

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
				data.at("DecoyTimes")
			};

			_stageDataItem = item;
		}

		return true;
	}

	STAGE_DATA_ITEM GetStageData()const { return _stageDataItem; }

	STAGE_DATA_ITEM _stageDataItem;
};