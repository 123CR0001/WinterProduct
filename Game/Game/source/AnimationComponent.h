#pragma once
#include"Component.h"
#include<deque>
#include<unordered_map>

class AnimationComponent :public Component {
public:
	AnimationComponent(class ObjectBase* owner,int order = 999);
	virtual ~AnimationComponent();

	bool Process()override;

	void LoadAnimation(const char* animName,const char* fileName,int loop);

	void ChangeAnimation(const char* animName);

	class Animation {
		friend class AnimationComponent;
	public:
		Animation() {}
		virtual ~Animation() {}
	protected:
		int _attachIndex;
		float _totalTime;
		float _playTime;
		// ���[�V�����u�����h�p
		float _closeTotalTime;
		float _closeTime;
		// ���[�v�J�E���g�p
		int _loopCnt;
	};

	//���[�V�����ł̏����̍ۂɁA�Đ����Ԃ��Q�Ƃ������B
	//
	//�Q�Ƃ���̂͐V����Animation�Ȃ̂ŁA_vAnim�̖�����_playTime���Q�Ƃ���B
	float GetPlayTime() const{ 
		if (_vAnim.size() > 0) {
			return (*_vAnim.back())._playTime;
		}
		return 0.f;
	}

	int GetAnimationAttachIndex() const{
		if (_vAnim.size() > 0) {
			return (*_vAnim.back())._attachIndex;
		}
		return 0;
	}

	const char* GetAnimationName() {
		return _playAnimationName;
	}

	std::deque<Animation*>& GetAnimations() { return _vAnim; }

protected:
	std::deque<Animation*> _vAnim;

	//�A�j���[�V�����p�̃n���h����o�^
	std::unordered_map<const char*, int>_animation;
	std::unordered_map<const char*, int>_animLoop;

	//�Đ����̃A�j���[�V�����̖��O
	const char* _playAnimationName;

	float _closeMaxTime;

	float _rate;
};
