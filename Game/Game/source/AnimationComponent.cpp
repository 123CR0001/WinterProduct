#include"AnimationComponent.h"
#include"ObjectBase.h"


AnimationComponent::AnimationComponent(ObjectBase* owner ,int order)
	:Component(owner,order) 
	,_playAnimationName(nullptr)
	,_rate(0.f)
{
	_closeMaxTime = 6.f;
}

AnimationComponent::~AnimationComponent(){
	for (int a = 0; a < _vAnim.size(); a++) {
		delete _vAnim[a];
	}
}

bool AnimationComponent::Process() {

	const int handle = _owner->GetHandle();

	// �A�j���[�V�������ԏ���
	for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ) {
		if ((*iteAnim)->_closeTime == 0.f) {
			// �Đ����Ԃ�i�߂�
			(*iteAnim)->_playTime += 1.f;
			// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
			if ((int)(*iteAnim)->_playTime >= (int)(*iteAnim)->_totalTime) {
				if ((*iteAnim)->_loopCnt > 1 || (*iteAnim)->_loopCnt == 0) {
					if ((*iteAnim)->_loopCnt > 1) {
						(*iteAnim)->_loopCnt--;
					}
					(*iteAnim)->_playTime = 0.0f;
				}
				else {
					(*iteAnim)->_playTime = (*iteAnim)->_totalTime;
				}
			}

			// �u�����h����ύX����
			MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, 1.f - _rate);
		}
		else {
			// �����Ԃ����炷
			(*iteAnim)->_closeTime -= 1.f;

			// �����Ԃ��I��������H
			if ((*iteAnim)->_closeTime <= 0.f) {
				_rate = 0.f;
				// �u�����h����ύX����
				MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, _rate);

				// �A�j���[�V�������A�^�b�`����Ă�����A�f�^�b�`����
				MV1DetachAnim(handle, (*iteAnim)->_attachIndex);
				// ���̃A�j���[�V�������폜
				delete (*iteAnim);
				iteAnim = _vAnim.erase(iteAnim);

				continue;
			}

			_rate = (*iteAnim)->_closeTime / (*iteAnim)->_closeTotalTime;

			// �u�����h����ύX����
			MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, _rate);
		}
		++iteAnim;
	}

	//�A�j���[�V�����̓K�p
	for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ++iteAnim) {
		MV1SetAttachAnimTime(handle, (*iteAnim)->_attachIndex, (*iteAnim)->_playTime);
	}

	return true;
}

//loop = 0�@���[�v 
//loop < n�@n��Đ� 
void AnimationComponent::LoadAnimation(const char* animName,const char* fileName,int loop) {
	_animation.emplace(animName, MV1GetAnimIndex(_owner->GetHandle(),fileName));
	_animLoop.emplace(animName,loop);
}

void AnimationComponent::ChangeAnimation(const char* animName) {

	//�Đ����Ă���A�j���[�V�����Ȃ�A�ύX���Ȃ�
	if(_playAnimationName == animName) { return; }

	//�ύX�������A�j���[�V�������o�^����Ă��邩
	if(_animation.find(animName) == _animation.end()) { return; }

	//���[�V������3�ȏ�u�����h����Ɗ댯�Ȃ̂ŁA2�ɂȂ�悤�ɂ���
	{	
		const int handle = _owner->GetHandle(); 
		while(_vAnim.size() > 1) {
			auto anim = _vAnim.front();
			// �A�j���[�V�������A�^�b�`����Ă�����A�f�^�b�`����
			MV1DetachAnim(handle, anim->_attachIndex);
			// ���̃A�j���[�V�������폜
			delete anim;
			_vAnim.pop_front();
		}
	}

	// �A�^�b�`����Ă���A�j���[�V�����ɁAclose���Ԃ�݂���
	for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ++iteAnim) {
		if ((*iteAnim)->_closeTime == 0.f) {
			(*iteAnim)->_closeTime = _closeMaxTime;		// �u�����h����t���[����
			(*iteAnim)->_closeTotalTime = (*iteAnim)->_closeTime;
			(*iteAnim)->_playTime = 0.f;
		}
	}
	// �V�����A�j���[�V������ǉ�
	Animation* anim = NEW Animation();
	anim->_attachIndex = MV1AttachAnim(_owner->GetHandle(), _animation[animName], -1, FALSE);

	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	anim->_totalTime = MV1GetAttachAnimTotalTime(_owner->GetHandle(), anim->_attachIndex);
	// �Đ����Ԃ�������
	anim->_playTime = 0.0f;
	anim->_closeTime = 0.0f;
	// ���[�v�񐔐ݒ�
	anim->_loopCnt = _animLoop[animName];
	// �A�j���[�V�����ǉ�
	_vAnim.emplace_back(anim);

	_playAnimationName = animName;
}