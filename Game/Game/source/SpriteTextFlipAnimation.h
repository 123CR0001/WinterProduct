#pragma once
#include"SpriteText.h"
class SpriteTextFlipAnimation :public SpriteText {
public:
	SpriteTextFlipAnimation(int flipInterval, bool isLoop);
	~SpriteTextFlipAnimation();

	void LoadDivText(const char* fileName, int AllNum, int XNum, int YNum, int XSize, int YSize) {
		_handles.resize(AllNum);
		ResourceServer::LoadDivGraph(fileName, AllNum, XNum, YNum, XSize, YSize, _handles.data());
	}

	void SetDivText(std::vector<int>handles) {
		_handles = handles;
	}

	void SetAnimSpeed(int flipInterval) {
		_flipInterval = flipInterval;
	}

	void Reverse() { _isReverse = !_isReverse; }

	void Draw(MATRIX mView = MGetIdent())override;

	//
	void Play() { _isPlayAnimation = true; }
	void Stop() { _isPlayAnimation = false; }

	//アニメーションが再生されている
	bool IsPlay() { return _isPlayAnimation; }

private:
	int _flipInterval;				//アニメーションの切り替え速度

	bool _isReverse;				//アニメーションを逆再生するか

	int _frameCnt;					//アニメーションのフレームカウント

	bool _isPlayAnimation;          //アニメーションをするか

	bool _isLoop;					//アニメーションをループさせるか

	std::vector<int>_handles;		//アニメーション画像のハンドル
};