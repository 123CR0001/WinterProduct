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

			// ブレンド率を変更する
			MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, 1.f - _rate);
		}
		else {
			// 閉じ時間を減らす
			(*iteAnim)->_closeTime -= 1.f;

			// 閉じ時間が終わったか？
			if ((*iteAnim)->_closeTime <= 0.f) {
				_rate = 0.f;
				// ブレンド率を変更する
				MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, _rate);

				// アニメーションがアタッチされていたら、デタッチする
				MV1DetachAnim(handle, (*iteAnim)->_attachIndex);
				// このアニメーションを削除
				delete (*iteAnim);
				iteAnim = _vAnim.erase(iteAnim);

				continue;
			}

			_rate = (*iteAnim)->_closeTime / (*iteAnim)->_closeTotalTime;

			// ブレンド率を変更する
			MV1SetAttachAnimBlendRate(handle, (*iteAnim)->_attachIndex, _rate);
		}
		++iteAnim;
	}

	//アニメーションの適用
	for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ++iteAnim) {
		MV1SetAttachAnimTime(handle, (*iteAnim)->_attachIndex, (*iteAnim)->_playTime);
	}

	return true;
}

//loop = 0　ループ 
//loop < n　n回再生 
void AnimationComponent::LoadAnimation(const char* animName,const char* fileName,int loop) {
	_animation.emplace(animName, MV1GetAnimIndex(_owner->GetHandle(),fileName));
	_animLoop.emplace(animName,loop);
}

void AnimationComponent::ChangeAnimation(const char* animName) {

	//再生しているアニメーションなら、変更しない
	if(_playAnimationName == animName) { return; }

	//変更したいアニメーションが登録されているか
	if(_animation.find(animName) == _animation.end()) { return; }

	//モーションを3つ以上ブレンドすると危険なので、2つになるようにする
	{	
		const int handle = _owner->GetHandle(); 
		while(_vAnim.size() > 1) {
			auto anim = _vAnim.front();
			// アニメーションがアタッチされていたら、デタッチする
			MV1DetachAnim(handle, anim->_attachIndex);
			// このアニメーションを削除
			delete anim;
			_vAnim.pop_front();
		}
	}

	// アタッチされているアニメーションに、close時間を設ける
	for (auto iteAnim = _vAnim.begin(); iteAnim != _vAnim.end(); ++iteAnim) {
		if ((*iteAnim)->_closeTime == 0.f) {
			(*iteAnim)->_closeTime = _closeMaxTime;		// ブレンドするフレーム数
			(*iteAnim)->_closeTotalTime = (*iteAnim)->_closeTime;
			(*iteAnim)->_playTime = 0.f;
		}
	}
	// 新しいアニメーションを追加
	Animation* anim = NEW Animation();
	anim->_attachIndex = MV1AttachAnim(_owner->GetHandle(), _animation[animName], -1, FALSE);

	// アタッチしたアニメーションの総再生時間を取得する
	anim->_totalTime = MV1GetAttachAnimTotalTime(_owner->GetHandle(), anim->_attachIndex);
	// 再生時間を初期化
	anim->_playTime = 0.0f;
	anim->_closeTime = 0.0f;
	// ループ回数設定
	anim->_loopCnt = _animLoop[animName];
	// アニメーション追加
	_vAnim.emplace_back(anim);

	_playAnimationName = animName;
}