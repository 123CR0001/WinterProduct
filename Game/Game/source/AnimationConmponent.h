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

	//モーションでの処理の際に、再生時間を参照したい。
	//
	//参照するのは新しいAnimationなので、_vAnimの末尾の_playTimeを参照する。
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
		// モーションブレンド用
		float _closeTotalTime;
		float _closeTime;
		// ループカウント用
		int _loopCnt;
	};
	std::vector<Animation*> _vAnim;

	//アニメーションを統一していいなら、staticにする
	//登録用関数の内容も変更
	std::unordered_map<const char*, int>_animation;
	std::unordered_map<const char*, int>_animLoop;

	bool _isChangeAnimation;
	const char* _changeAnimationName;
};
