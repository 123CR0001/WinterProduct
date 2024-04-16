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

	//�A�j���[�V�������Đ�����Ă���
	bool IsPlay() { return _isPlayAnimation; }

private:
	int _flipInterval;				//�A�j���[�V�����̐؂�ւ����x

	bool _isReverse;				//�A�j���[�V�������t�Đ����邩

	int _frameCnt;					//�A�j���[�V�����̃t���[���J�E���g

	bool _isPlayAnimation;          //�A�j���[�V���������邩

	bool _isLoop;					//�A�j���[�V���������[�v�����邩

	std::vector<int>_handles;		//�A�j���[�V�����摜�̃n���h��
};