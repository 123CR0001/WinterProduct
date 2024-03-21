#include"MotionComponent.h"
#include"CharaBase.h"
#include"AnimationComponent.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"ModeEffekseer.h"

#include"ApplicationGlobal.h"

MotionComponent::MotionComponent(AnimationComponent* owner,int order)
	:Component(owner->GetOwner(), order)
	, _anim(owner)
	, _motCnt(0) 
{
	_commandFuncMap["MOVE"] = [this](const MOTION_DATA_ITEM& item) {
		
		float rad = DegToRad(item.vector) + _anim->GetOwner()->GetEulerAngle().y;

		Vector3 v(sin(rad), 0, cos(rad));
		v.Normalized();

		_anim->GetOwner()->AddPos( v * item.vectorScale);

		_motCnt++; 
	};

	_commandFuncMap["LOOP"] = [this](const MOTION_DATA_ITEM& item) {_motCnt = 0; };

	//_commandFuncMap["CHANGE_MOTION"] = [this](const MOTION_DATA_ITEM& item) {
	//	_chara->GetAnimationComponent()->ChangeAnimation(item.ChangeMotion.c_str());
	//	_motCnt;
	//};

	_commandFuncMap["PLAY_SOUND"] = [this](const MOTION_DATA_ITEM& item) {//SE�̍Đ�
	
		std::string name = item.soundPlayName;

		std::vector<std::string> names;

		int frontNum = 0;

		while(frontNum != -1) {

			frontNum = name.find_first_of(",");

			std::cout << frontNum << std::endl;

			names.emplace_back(name.substr(0, frontNum));

			name = name.substr(frontNum + 1, name.size());
		}

		gGlobal._sndServer.Play(names[rand() % names.size()]);

		auto snd = gGlobal._sndServer.Get(names[rand() % names.size()]);

		if (snd) {
			Set3DRadiusSoundMem(300.f, snd->GetSoundHandle());					// �������鋗��
			Set3DPositionSoundMem(_owner->GetDxPos(), snd->GetSoundHandle());	// �Đ��ʒu���L�����ʒu��
		}

		_motCnt++;
	};

	_commandFuncMap["PLAY_EFFECT_3DFRAME"] = [this](const MOTION_DATA_ITEM& item) {
		//�G�t�F�N�g�̍Đ�
		void* mode = ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME);
		if (mode) {

			// �t���[��������t���[���ԍ����擾����
			int FrameIndex = MV1SearchFrame(_anim->GetOwner()->GetHandle(), item.effectPlay3DFrame.c_str());

			// �t���[���̌��݂̃��[���h�ł̏�Ԃ������s����擾����
			MATRIX FrameMatrix = MV1GetFrameLocalWorldMatrix(_anim->GetOwner()->GetHandle(), FrameIndex);

			ModeEffekseer* effect = _anim->GetOwner()->GetObjectServer()->GetGame()->GetModeEffekseer();

			Vector3 angle = _anim->GetOwner()->GetEulerAngle();

			effect->Play(
				item.effectPlayName,
				Vector3(FrameMatrix.m[3][0], FrameMatrix.m[3][1], FrameMatrix.m[3][2]),
				Vector3(angle.x, angle.y + PI, angle.z)
			);
		}

		_motCnt++;
	};
		/*		{"ATTACK_ON",MotionCommand::ATTACK_ON},
		{"ATTACK_OFF",MotionCommand::ATTACK_OFF},
		{"PLAY_EFFECT",MotionCommand::PLAY_EFFECT},
		{"PLAY_EFFECT_3DFRAME",MotionCommand::PLAY_EFFECT_3DFRAME},
		{"PLAY_SOUND",MotionCommand::PLAY_SOUND},
		{"LOOP",MotionCommand::LOOP},
		{"CHANGE_MOTION",MotionCommand::CHANGE_MOTION},
		{"MOVE",MotionCommand::MOVE}*/
}

MotionComponent::~MotionComponent(){}

bool MotionComponent::Process() {

	auto motionData = gGlobal._charaMotionData;

	//���̃R���|�[�l���g��ێ�����I�[�i�[�̖��O
	std::string objectName = _anim->GetOwner()->GetName();
	AnimationComponent* anim = _anim;

	//�A�j���[�V������������������Ă��Ȃ�������A���������Ȃ�
	if(!anim->GetAnimationName()) { return false; }

	std::string animName = anim->GetAnimationName();

	//CharaBase���Đ����Ă���A�j���[�V�����̍Đ����Ԃ��擾		�؂�グ
	int playTime = static_cast<int>(anim->GetPlayTime() + 0.999f);

	//���[�V�������؂�ւ���Ă�����A_motCnt��0�ɂ���
	if(_oldAnimName != animName) {
		_motCnt = 0;
	}

	//�X�V
	_oldAnimName = animName;

	//���[�V�����f�[�^���o�^����Ă��Ȃ���
	if (motionData.find(objectName) == motionData.end()) { return false; }
	if (motionData[objectName].find(animName) == motionData[objectName].end()) { return false; }

	//���[�V�����ɓo�^�����f�[�^��vector�R���e�i��_motCnt�ŎQ�Ƃ��Ă��邽�߁A_motCnt�����̃R���e�i�̃T�C�Y�ȏ�ɂȂ�Ȃ��悤�ɂ���B
	if (motionData[objectName][animName].size() <= _motCnt) {
		_motCnt = 0;
	}

	if(_motCnt == 61) {
		int a = 0;
	}

	//���[�V�����f�[�^�Ŏw�肳�ꂽ�Đ����Ԃƈꏏ��
	if (playTime == motionData[objectName][animName][_motCnt].playTime) {

		const auto& data = motionData[objectName][animName][_motCnt];

		//���ʂ̃R�}���h������
		if (_commandFuncMap.find(data.command) != _commandFuncMap.end()) {
			(_commandFuncMap[data.command])(data);
			return true;
		}

		//�L�����ŗL�̃R�}���h������
		if (_customCommandFuncMap.find(data.command) != _customCommandFuncMap.end()){
			(_customCommandFuncMap[data.command])(data);
			return true;
		}

	}

	return true;
}
