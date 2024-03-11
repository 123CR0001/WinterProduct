#pragma once
#include"appframe.h"
#include"Animation2D.h"

class ModeLightsOut :public ModeBase {
public:
	ModeLightsOut(class ModeGame* game);
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;
protected:
	//��ʂɓ\��G�t�F�N�g
	int _cg;
	//��L�̃G�t�F�N�g�̓����x
	int _alpha;

	//��ʂɓ\��G�t�F�N�g�̏�ɂ�����m�C�Y�̃A�j���[�V����
	Animation2D* _noiseAnim;

	int _textHud;

	//��������
	int _frameCnt;

	int _oldEnemyNum;

	//SE�̍Đ��Ԋu
	int _frameCntSE;

	class ModeGame* _game;
};