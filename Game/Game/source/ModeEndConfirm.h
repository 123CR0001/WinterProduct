#pragma once

#include "appframe.h"

class ModeEndConfirm : public ModeBase
{
	typedef ModeBase base;
	typedef ResourceServer res;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:
	// UIProcess�N���X
	class		UIServer* _ui;
	// �I�����̑���
	const int	MAX_OPTIONS = 2;
	// UI�̃��C���[�l
	// value�F10
	const int LOW_LAYER_VALUE = 40;
	// value�F20
	const int BASIC_LAYER_VALUE = 50;
	// value�F30
	const int HIGH_LAYER_VALUE = 60;

	// ����I�����Ă��邩
	int			_select;
};

