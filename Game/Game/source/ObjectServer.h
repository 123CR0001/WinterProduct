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
	std::vector<class CommonSoldier*>& GetCommonSoldiers() { return _commonSoldiers; }

	bool SetNavigationModel(const char* fileName,const char* attacFrameIndex) {
		_navigationModel = MV1LoadModel(fileName); 
		_navigationAttachIndex = MV1SearchFrame(_navigationModel, attacFrameIndex);
		return true;
	}
	int GetNavigationHandle()const { return _navigationModel; }
	int GetNavigationAttachIndex()const { return _navigationAttachIndex; }

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
	std::vector<class CommonSoldier*> _commonSoldiers;		

	class PhysWorld* _physWorld;

	class Player* _player;

	int _navigationModel;				//UE�̃i�r�Q�[�V�������b�V���@
	int _navigationAttachIndex;

	//ModeGame�����@�\����𗘗p���邽�߂ɁA���g��new����ModeGame�N���X�̃A�h���X�ƕێ�
	class ModeGame* _game;
};