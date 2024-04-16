#pragma once
#include"ObjectBase.h"

class TracerSpawner :public ObjectBase {
public:
	TracerSpawner(class ObjectServer* server);
	~TracerSpawner();

	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;

	std::vector<Vector3>& GetPoints() { return _playerPoints; }

private:
	unsigned int _elapsedTime;				//�o�ߎ���
	unsigned int _spawnTime;				//�g���[�T�[�̐�������

	std::vector<Vector3>_playerPoints;

	//�v���C���[�̍��W���L�^����Ԋu
	unsigned int _frameCnt;					//�t���[���J�E���g
	const unsigned int _interval;			//�Ԋu	
};