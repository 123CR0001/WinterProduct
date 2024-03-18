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

	//���̊֐��𗘗p���āA�e�N���X�Œǉ����ꂽ�A�h���X��o�^����
	std::vector<class CharaBase*>& GetCharas() { return _charas; }
	std::vector<class ObjectBase*>& GetEnemys() { return _enemys; }
	std::vector<class CommonSoldier*>& GetCommonSoldiers() { return _commonSoldiers; }
	std::vector<class Siren*>& GetSirens() { return _sirens; }

	bool LoadData(std::string stageName);

	class Navi* GetNavi()const { return _navi; }

	class PhysWorld* GetPhysWorld()const { return _physWorld; }

	//_objects _add _delete �̃R���e�i���ێ�����A�h���X��delete�A�T�C�Y���O�ɂ���
	bool ClearObject();

private:
	std::vector<class ObjectBase*> _objects;
	std::vector<class ObjectBase*> _addObj;
	std::vector<class ObjectBase*> _deleteObj;

	//����̃N���X�����~�����̂ŁA�A�h���X�o�^�p�̔z��
	//�p�r�͓����蔻���ObjectBase*�^�̃|�C���^�������̃N���X�����ł��Ȃ��������_�E���L���X�g�����ɂ��邽��
	std::vector<class CharaBase*> _charas;		
	std::vector<class ObjectBase*>_enemys;
	std::vector<class CommonSoldier*> _commonSoldiers;
	std::vector<class Siren*>_sirens;

	class PhysWorld* _physWorld;

	class Player* _player;

	class Navi* _navi;

	//ModeGame�����@�\����𗘗p���邽�߂ɁA���g��new����ModeGame�N���X�̃A�h���X�ƕێ�
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