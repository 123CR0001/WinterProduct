#include"TraserSpawner.h"
#include"ObjectServer.h"

#include"Traser.h"
#include"Player.h"

TraserSpawner::TraserSpawner(ObjectServer* owner)
	:ObjectBase(owner)
	,_elapsedTime(0)
	,_spawnTime(300)
	//�v���C���[�̍ŏ��̍��W���擾����������
	,_frameCnt(30)
	,_interval(30)
{

}

TraserSpawner::~TraserSpawner() {

}

bool TraserSpawner::Initialize() {
	return true;
}

bool TraserSpawner::Terminate() {
	return true;
}

bool TraserSpawner::Process() {
	//��莞�Ԍo�߂�����A�g���[�T�[���X�|�[��
	if (_elapsedTime == _spawnTime) {
		NEW Traser(this);
		//��񂫂�ł����̂ŁA���g���폜
		GetObjectServer()->DeleteObject(this);
	}

	_elapsedTime++;


	if (_frameCnt == _interval) {
		_frameCnt = 0;
		_playerPoints.emplace_back(GetObjectServer()->GetPlayer()->GetPos());
	}
	_frameCnt++;


	return true;
}

bool TraserSpawner::Render() {
	return true;
}