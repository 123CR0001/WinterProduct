#include "appframe.h"
#include"MotionData.h"
#include "SoundServer.h"

class ApplicationGlobal
{
public:
	ApplicationGlobal();
	virtual ~ApplicationGlobal();

	//���[�V�����f�[�^�̓ǂݍ���
	void LoadMotionData();

	bool Init();

public:
	//�L�����̎�ނɂ��o�^
	std::unordered_map<std::string/*�L������*/, MOTION_DATA> _charaMotionData;

	SoundServer	_sndServer;

}; 

// ���̃\�[�X��gGlobal���g����悤��
extern ApplicationGlobal		gGlobal;
