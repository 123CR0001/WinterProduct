#pragma once
class LightsOut {
public:
	LightsOut(class ModeGame* game);
	~LightsOut();

	bool Process();

	//��Ԃ����������̎��ɂ���
	void Use() { if(_state==STATE::kNone)_state = STATE::kStart; }

	//��Ԃ������̎�����
	void Stop() { if(_state == STATE::kProcess)_state = STATE::kEnd; }

	//���C�c�A�E�g���g�p�ł����Ԃ�
	bool IsUse() { return _state == STATE::kNone && _useTimes > 0; }

	//�������Ԃ����Z
	void AddLimitCount() { _limitCnt += 120; }

private:
	class SpriteTextFlipAnimation*	_timerBg;			//�b���̔w�i
	class SpriteTextFlipAnimation*	_noise;				//�G�t�F�N�g
	class SpriteTextFlipAnimation*	_isUseLightsOut;	//���C�c�A�E�g���g�p�\�ɂȂ�������\������UI 
	class SpriteText*				_hud;				//HUD
	class UISecMiliSec*				_timer;				//�������ԕ\��UI

	class ModeGame*					_game;				//�I�[�i�[

	class CreateAfterImageComponent* _afterImageCom;	//�v���C���[�ɒǉ�����R���|�[�l���g�̃A�h���X��ێ�����

	int _limitCnt;										//��������	
	int _useTimes;										//���C�c�A�E�g�̎g�p��	
	int _oldEnergyCount;								//�O�t���[���̃G�l���M�[�̒l

	//�����X�e�b�v
	enum class STATE {
		kNone,
		kStart,
		kProcess,
		kEnd
	}_state;
};