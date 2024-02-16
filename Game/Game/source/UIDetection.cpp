
#include "ApplicationMain.h"
#include "UIDetection.h"
#include "ModeGame.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"

UIDetection::UIDetection(class ModeGame* game):_game(game) {
	// ������
	_eneInfo.bFind = false;
	_eneInfo.val = 100;

	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();
	for (auto iter : CommonSoldiers) {
		_vEneInfo.push_back(_eneInfo);
	}
}

void UIDetection::Process() {
	// �G�Ɍ���������_bFind��true�ɂ���
	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();
	int i = 0;
	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {
		if ((*iter)->IsPlayerFound()) { 
			_vEneInfo[i].bFind = true;
		}
		else if(!(*iter)->IsPlayerFound()){
			_vEneInfo[i].bFind = false;
		}
		i++;
	}
	for (int j = 0; j < _vEneInfo.size(); j++) {
		// �������Ă���Ԃ̓Q�[�W���㏸����
		if (_vEneInfo[j].bFind) { _vEneInfo[j].val--; }
		// ���E����O�ꂽ��A�Q�[�W����������
		else {
			if (_vEneInfo[j].val < 100) { _vEneInfo[j].val++; }
		}
		_vEneInfo[j].val = Clamp(0, 100, _vEneInfo[j].val);
	}
	
}

void UIDetection::Render() {
	// �摜�ǂݍ���
	int cgBaseCircle = ResourceServer::LoadGraph("res/UI/Game/cir.png");
	int cgQuestion = ResourceServer::LoadGraph("res/UI/Game/question.png");
	int cgSuprise = ResourceServer::LoadGraph("res/UI/Game/surprise.png");

	// player�𔭌����Ă���G��������
	auto CommonSoldiers = _game->GetObjectServer()->GetCommonSoldiers();
	int i = 0;
	for (auto iter = CommonSoldiers.begin(); iter != CommonSoldiers.end(); ++iter) {
		if ((*iter)->IsPlayerFound() || _vEneInfo[i].val < 100) {										// �G�ɔ�������Ă���A�����͌��m�x���c���Ă�����`�悷��
			VECTOR vPos = (*iter)->GetDxPos();													// �G�̈ʒu���W�����o��
			VECTOR highVec = { 0,230,0 };														// 
			VECTOR eneOverhead = VAdd(vPos, highVec);											// �G�̓���ɕ\�����邽�߂Ƀx�N�g�������Z����
			VECTOR convertScreenPos = ConvWorldPosToScreenPos(eneOverhead);						// ���[���h���W���X�N���[�����W�ɕϊ�����
																								// 
			DrawGraph(convertScreenPos.x - 50, convertScreenPos.y - 50, cgBaseCircle, TRUE);	// �x�[�X�p�̐F�t���T�[�N��
			DrawCircleGauge(convertScreenPos.x, convertScreenPos.y, _vEneInfo[i].val, _cg);					// ���ۂɃQ�[�W�̏㏸�E������`�悷��T�[�N���@��
			if (_vEneInfo[i].val == 0) {
				DrawGraph(convertScreenPos.x - 40, convertScreenPos.y - 40, cgSuprise, TRUE);	// �u�I�v��������
			}
			else {
				DrawGraph(convertScreenPos.x - 40, convertScreenPos.y - 40, cgQuestion, TRUE);	// �u�H�v�^��
			}
		}
		i++;
	}
}
