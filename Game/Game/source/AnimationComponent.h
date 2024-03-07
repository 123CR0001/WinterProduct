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
		// モーションブレンド用
		float _closeTotalTime;
		float _closeTime;
		// ループカウント用
		int _loopCnt;
	};

	//モーションでの処理の際に、再生時間を参照したい。
	//
	//参照するのは新しいAnimationなので、_vAnimの末尾の_playTimeを参照する。
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

	//アニメーション用のハンドルを登録
	std::unordered_map<const char*, int>_animation;
	std::unordered_map<const char*, int>_animLoop;

	//再生中のアニメーションの名前
	const char* _playAnimationName;

	float _closeMaxTime;

	float _rate;
};
