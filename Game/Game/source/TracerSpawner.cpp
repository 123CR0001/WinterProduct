#include"TracerSpawner.h"
#include"ObjectServer.h"

#include"Tracer.h"
#include"Player.h"

TracerSpawner::TracerSpawner(ObjectServer* owner)
	:ObjectBase(owner)
	,_elapsedTime(0)
	,_spawnTime(10800)
	//�v���C���[�̍ŏ��̍��W���擾����������
	,_frameCnt(3)
	,_interval(3)
{

}

TracerSpawner::~TracerSpawner() {

}

bool TracerSpawner::Initialize() {
	ObjectBase::Initialize();
	return true;
}

bool TracerSpawner::Terminate() {
	return true;
}

bool TracerSpawner::Process() {
	//��莞�Ԍo�߂�����A�g���[�T�[���X�|�[��
	if (_elapsedTime == _spawnTime) {
		NEW Tracer(this);
		//��񂫂�ł����̂ŁA���g���폜
		GetObjectServer()->DeleteObject(this);
	}

	//�o�ߎ��Ԃ��J�E���g
	_elapsedTime++;

	//�v���C���[�̍��W��o�^
	if (_frameCnt == _interval) {
		_frameCnt = 0;
		_playerPoints.emplace_back(GetObjectServer()->GetPlayer()->GetPos());
	}

	//�t���[���J�E���g��i�߂�
	_frameCnt++;


	return true;
}

bool TracerSpawner::Render() {
	return true;
}