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

	/*アニメーション遷移時*/
	if (_isChangeAnimation) {
		//変更したいアニメーションが登録されているか
		if (_animation.find(_changeAnimationName) != _animation.end()) {
			// アタッチされているアニメーションに、close時間を設ける
			for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ++iteAnim) {
				if ((*iteAnim)->_closeTime == 0.f) {
					(*iteAnim)->_closeTime = 6.f;		// ブレンドするフレーム数
					(*iteAnim)->_closeTotalTime = (*iteAnim)->_closeTime;
				}
			}
			// 新しいアニメーションを追加
			Animation* anim = NEW Animation();
			anim->_attachIndex = MV1AttachAnim(handle,_animation[_changeAnimationName], -1, FALSE);

			// アタッチしたアニメーションの総再生時間を取得する
			anim->_totalTime = MV1GetAttachAnimTotalTime(handle, anim->_attachIndex);
			// 再生時間を初期化
			anim->_playTime = 0.0f;
			anim->_closeTime = 0.0f;
			// ループ回数設定
			anim->_loopCnt = _animLoop[_changeAnimationName];
			// アニメーション追加
			_vAnim.push_back(anim);
		}
		//
		_isChangeAnimation = false;
	}

	// アニメーション時間処理
	for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ) {
		if ((*iteAnim)->_closeTime == 0.f) {
			// 再生時間を進める
			(*iteAnim)->_playTime += 1.f;
			// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
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
			// 閉じ時間を減らす
			(*iteAnim)->_closeTime -= 1.f;

			// 閉じ時間が終わったか？
			if ((*iteAnim)->_closeTime <= 0.f) {
				// アニメーションがアタッチされていたら、デタッチする
				MV1DetachAnim(handle, (*iteAnim)->_attachIndex);
				// このアニメーションを削除
				delete (*iteAnim);
				iteAnim = _vAnim.erase(iteAnim);
				continue;
			}
			// ブレンド率を変更する
			MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, (*iteAnim)->_closeTime / (*iteAnim)->_closeTotalTime);
		}
		++iteAnim;
	}

	//アニメーションの適用
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