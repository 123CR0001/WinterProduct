#pragma once
#include"SpriteText.h"
class SpriteTextFlipAnimation :public SpriteText {
public:
	SpriteTextFlipAnimation(int flipInterval, bool isLoop);
	~SpriteTextFlipAnimation();

	void LoadDivText(const char* fileName, int AllNum, int XNum, int YNum, int XSize, int YSize) {
		_handles.resize(AllNum);
		LoadDivGraph(fileName, AllNum, XNum, YNum, XSize, YSize, _handles.data());
	}

	void SetDivText(std::vector<int>handles) {
		_handles = handles;
	}

	void SetAnimSpeed(int flipInterval) {
		_flipInterval = flipInterval;
	}

	void Reverse() { _isReverse = !_isReverse; }

	void Draw(MATRIX mView = MGetIdent())override;


	void Play() { _isPlayAnimation = true; }
	void Stop() { _isPlayAnimation = false; }

	bool IsPlay() { return _isPlayAnimation; }

private:
	//�A�j���[�V����(�摜�̐؂�ւ�)�̊Ԋu
	int _flipInterval;

	//�t�Đ����邩
	bool _isReverse;

	int _frameCnt;

	//�A�j���[�V���������邩
	bool _isPlayAnimation;

	//�A�j���[�V���������[�v�����邩
	bool _isLoop;

	std::vector<int>_handles;
};