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
	std::vector<class CommonSoldier*>& GetCommonSoldiers() { return _commonSoldiers; }

	bool SetNavigationModel(const char* fileName,const char* attacFrameIndex) {
		_navigationModel = MV1LoadModel(fileName); 
		_navigationAttachIndex = MV1SearchFrame(_navigationModel, attacFrameIndex);
		return true;
	}
	int GetNavigationHandle()const { return _navigationModel; }
	int GetNavigationAttachIndex()const { return _navigationAttachIndex; }

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
	std::vector<class CommonSoldier*> _commonSoldiers;		

	class PhysWorld* _physWorld;

	class Player* _player;

	int _navigationModel;				//UEのナビゲーションメッシュ　
	int _navigationAttachIndex;

	//ModeGameが持つ機能や情報を利用するために、自身をnewしたModeGameクラスのアドレスと保持
	class ModeGame* _game;
};