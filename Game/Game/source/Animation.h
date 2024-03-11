#pragma once
#include"appframe.h"
#include<functional>

class Animation {
public:
	Animation(class SpriteText* text, int frame);
	virtual ~Animation();

	virtual bool Process();

	void Reverse() { _isEnd = false; _isReverse = true; }

	bool IsEnd()const { return _isEnd; }

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

	//�A�j���[�V�����ӂ��I��������
	bool _isEnd;

	//�A�j���[�V�������I�������ASpriteText����폜���邩�̕ϐ����~���������H
	//bool _isEndDelete;

};