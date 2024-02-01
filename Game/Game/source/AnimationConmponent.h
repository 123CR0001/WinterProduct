#pragma once
#include"Component.h"
#include<vector>
#include<unordered_map>

class AnimationComponent :public Component {
public:
	AnimationComponent(class ObjectBase* owner,int order = 999);
	virtual ~AnimationComponent();

	bool Process()override;

	void LoadAnimation(const char* animName,const char* fileName,int loop);

	void ChangeAnimation(const char* animName);

	//���[�V�����ł̏����̍ۂɁA�Đ����Ԃ��Q�Ƃ������B
	//
	//�Q�Ƃ���̂͐V����Animation�Ȃ̂ŁA_vAnim�̖�����_playTime���Q�Ƃ���B
	float GetPlayTime() { 
		if (_vAnim.size() > 0) {
			return (*_vAnim.back())._playTime;
		}
		return 0.f;
	}

protected:

	

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
	std::vector<Animation*> _vAnim;

	//�A�j���[�V�����𓝈ꂵ�Ă����Ȃ�Astatic�ɂ���
	//�o�^�p�֐��̓��e���ύX
	std::unordered_map<const char*, int>_animation;
	std::unordered_map<const char*, int>_animLoop;

	bool _isChangeAnimation;
	const char* _changeAnimationName;
};
