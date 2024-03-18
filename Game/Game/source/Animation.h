#pragma once
#include"appframe.h"
#include<functional>

class Animation {
public:
	Animation(class SpriteText* text, int frame);
	virtual ~Animation();

	virtual bool Process();

	void Reverse() {
		_isEnd = false; _isReverse = true;
	}

	void SetFunc(std::function<void()>func) { _func = func; }

	bool IsEnd()const { return _isEnd; }

	void End() {

		if (!_isReverse) {
			_frameCnt = _frame;
		}
		else {
			_frameCnt = 0;
		}
		_isEnd = true;
	}

protected:
	class SpriteText* _text;

	//�J�E���g
	int _frameCnt;

	//���J�E���g��
	const int _frame;

	//�A�j���[�V�����̍Đ�����
	float _rate;

	//�A�j���[�V�������t�Đ�
	bool _isReverse;

	//�A�j���[�V�������I��������
	bool _isEnd;

	//�A�j���[�V�������I�������ASpriteText����폜���邩�̕ϐ����~���������H
	//bool _isEndDelete;

	//�A�j���[�V�������I�������A���鏈��
	std::function<void()>_func;

};