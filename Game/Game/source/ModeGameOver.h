#pragma once
#include "appframe.h"
#include "ModeGame.h"

class ModeGameOver :public ModeBase {
	typedef ModeBase base;
	typedef ResourceServer res;
public:
	ModeGameOver(ModeGame* game);

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

public:
	std::string GetStage() { return _stage; }	// �X�e�[�W�����擾

protected:
	ModeGame* _game;	// �Q�[�����[�h�ւ̃|�C���^
	std::string _stage;	// �X�e�[�W��

	class UIServer* _uiServer;	// UI�T�[�o�[

	const int MAX_OPTIONS = 2;	// ���ڂ̍ő吔
	const int LOW_LAYER_VALUE = 10;		// ���C���[�l�i10�j
	const int BASIC_LAYER_VALUE = 20;	// ���C���[�l�i20�j
	const int HIGH_LAYER_VALUE = 30;	// ���C���[�l�i30�j

	int _select;	// �I�𒆂̍���

#define BG_NUM 6
	int _cgBg[BG_NUM];	// �w�i
	int _animCnt;		// �A�j���[�V�����J�E���^
	int _frameCnt;		// �t���[���J�E���^
	int _motCnt;		// ���[�V�����J�E���^
};