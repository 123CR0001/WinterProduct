#pragma once
#include"appframe.h"
#include<functional>

class Animation {
public:
	Animation(class SpriteText* text, int frame);
	virtual ~Animation();

	virtual bool Process();

	//�A�j���[�V�������t�Đ�
	void Reverse() {
		_isEnd = false; _isReverse = true;
	}

	//�A�j���[�V�������I��������A���鏈����ݒ�
	void SetFunc(std::function<void()>func) { _func = func; }

	//�A�j���[�V�������I��������
	bool IsEnd()const { return _isEnd; }

	//�A�j���[�V�������I��
	void SkipEnd() {

		if (!_isReverse) {
			_frameCnt = _frame;
		}
		else {
			_frameCnt = 0;
		}
		_isEnd = true;
	}

protected:
	class SpriteText* _text;		//�A�j���[�V����������SpriteText

	int _frameCnt;					//�t���[���J�E���g

	const int _frame;				//�A�j���[�V�����̍ő�t���[��

	float _rate;					//�A�j���[�V�����̐i�s�x

	bool _isReverse;				//�t�Đ���	

	bool _isEnd;					//�A�j���[�V�������I��������

	std::function<void()>_func;		//�A�j���[�V�������I��������A���鏈��

};