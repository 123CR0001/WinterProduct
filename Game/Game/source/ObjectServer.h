#pragma once
#include<vector>
#include"appframe.h"

class ObjectServer {

public:
	ObjectServer(class ModeGame* game);
	~ObjectServer();

	void AddObject(class ObjectBase* obj);
	void DeleteObject(class ObjectBase* obj);

	bool Initialize();		//������
	bool Terminate();		//�I������
	bool ProcessInit();		//�����̎�O�ɌĂяo��������
	bool Process();			//����
	bool Renderer();		//�`��

	//�I�[�i�[
	class ModeGame* GetGame()const { return _game; }

	//�v���C���[
	class Player* GetPlayer() { return _player; }
	void SetPlayer(class Player* set) { _player = set; }

	//�I�u�W�F�N�g�̃R���e�i
	std::vector<class ObjectBase*> GetObjects()const { return _objects; }

	//���̊֐��𗘗p���āA�e�N���X�Œǉ����ꂽ�A�h���X��o�^����
	std::vector<class CharaBase*>& GetCharas() { return _charas; }
	std::vector<class CommonSoldier*>& GetCommonSoldiers() { return _commonSoldiers; }
	std::vector<class Siren*>& GetSirens() { return _sirens; }

	//�I�u�W�F�N�g�̃��C�A�E�g��ǂݍ��݁A�I�u�W�F�N�g�𐶐�
	bool LoadData(std::string stageName);

	//�i�r�N���X�̎擾
	class Navi* GetNavi()const { return _navi; }

	//�����N���X���擾
	class PhysWorld* GetPhysWorld()const { return _physWorld; }

	//_objects _add _delete �̃R���e�i���ێ�����A�h���X��delete�A�e�R���e�i�̃T�C�Y���O�ɂ���
	bool ClearObject();

private:
	//�I�u�W�F�N�g
	std::vector<class ObjectBase*> _objects;
	//�ǉ�����I�u�W�F�N�g�̃A�h���X
	std::vector<class ObjectBase*> _addObj;
	//�폜����I�u�W�F�N�g�̃A�h���X
	std::vector<class ObjectBase*> _deleteObj;

	//����̃N���X�����~�����̂ŁA�A�h���X�o�^�p�̔z��
	//�p�r�͓����蔻���ObjectBase*�^�̃|�C���^�������̃N���X�����ł��Ȃ��������_�E���L���X�g�����ɂ��邽��
	std::vector<class CharaBase*> _charas;		
	std::vector<class ObjectBase*>_enemys;
	std::vector<class CommonSoldier*> _commonSoldiers;
	std::vector<class Siren*>_sirens;

	//�����N���X
	class PhysWorld* _physWorld;

	//�v���C���[�N���X
	class Player* _player;

	//�i�r�N���X
	class Navi* _navi;

	//ModeGame�����@�\����𗘗p���邽�߂ɁA���g��new����ModeGame�N���X�̃A�h���X�ƕێ�
	class ModeGame* _game;

};

//�X�e�[�W���Ƃɕς��p�����[�^�[
struct STAGE_DATA_ITEM {
	int decoyTimes = 0;
	std::string nextStageName;
};

//STAGE_DATA_ITEM�̓ǂݍ��ݗpJson�N���X
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