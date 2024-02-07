#include"AnimationConmponent.h"
#include"ObjectBase.h"


AnimationComponent::AnimationComponent(ObjectBase* owner ,int order)
	:Component(owner,order) 
{

}

AnimationComponent::~AnimationComponent(){
	for (int a = 0; a < _vAnim.size(); a++) {
		delete _vAnim[a];
	}
}

bool AnimationComponent::Process() {

	int handle = _owner->GetHandle();

	/*�A�j���[�V�����J�ڎ�*/
	if (_isChangeAnimation) {
		//�ύX�������A�j���[�V�������o�^����Ă��邩
		if (_animation.find(_changeAnimationName) != _animation.end()) {
			// �A�^�b�`����Ă���A�j���[�V�����ɁAclose���Ԃ�݂���
			for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ++iteAnim) {
				if ((*iteAnim)->_closeTime == 0.f) {
					(*iteAnim)->_closeTime = 6.f;		// �u�����h����t���[����
					(*iteAnim)->_closeTotalTime = (*iteAnim)->_closeTime;
				}
			}
			// �V�����A�j���[�V������ǉ�
			Animation* anim = NEW Animation();
			anim->_attachIndex = MV1AttachAnim(handle,_animation[_changeAnimationName], -1, FALSE);

			// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
			anim->_totalTime = MV1GetAttachAnimTotalTime(handle, anim->_attachIndex);
			// �Đ����Ԃ�������
			anim->_playTime = 0.0f;
			anim->_closeTime = 0.0f;
			// ���[�v�񐔐ݒ�
			anim->_loopCnt = _animLoop[_changeAnimationName];
			// �A�j���[�V�����ǉ�
			_vAnim.push_back(anim);
		}
		//
		_isChangeAnimation = false;
	}

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
		}
		else {
			// �����Ԃ����炷
			(*iteAnim)->_closeTime -= 1.f;

			// �����Ԃ��I��������H
			if ((*iteAnim)->_closeTime <= 0.f) {
				// �A�j���[�V�������A�^�b�`����Ă�����A�f�^�b�`����
				MV1DetachAnim(handle, (*iteAnim)->_attachIndex);
				// ���̃A�j���[�V�������폜
				delete (*iteAnim);
				iteAnim = _vAnim.erase(iteAnim);
				continue;
			}
			// �u�����h����ύX����
			MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, (*iteAnim)->_closeTime / (*iteAnim)->_closeTotalTime);
		}
		++iteAnim;
	}

	//�A�j���[�V�����̓K�p
	for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ++iteAnim) {
		MV1SetAttachAnimTime(handle, (*iteAnim)->_attachIndex, (*iteAnim)->_playTime);
	}

	_owner->ModelMatrixSetUp();

	return true;
}

void AnimationComponent::LoadAnimation(const char* animName,const char* fileName,int loop) {
	_animation.emplace(animName, MV1GetAnimIndex(_owner->GetHandle(),fileName));
	_animLoop.emplace(animName,loop);
}

void AnimationComponent::ChangeAnimation(const char* animName) {
	_isChangeAnimation = true;
	_changeAnimationName = animName;
}