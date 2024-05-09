#pragma once
#include<string>
	//�N���A��ʂ̃��U���g�f�[�^
class ClearData {
public:
	ClearData() {
		maxCombo = 0;
		clearSecondTime = 0.f;
		maxDetectionLevel = 0.f;
	}
	~ClearData() {}

	//�ō��R���{��
	int maxCombo;

	//�N���A�^�C��
	int clearSecondTime;

	//�ō����m�x
	float maxDetectionLevel;

	//���ۂɂ̓��U���g�ɂ́A�֌W�̂Ȃ��f�[�^
	//���̃X�e�[�W���ʏ��
	std::string _stageName;

	//���̃X�e�[�W�̎��ʏ��
	std::string _nextStageName;

	//��L�̃f�[�^���烉���N���Z�o
	const char* GetRank() {
		int point = 0;
		//����
		if (clearSecondTime < 60 * 1) {			//�]��S
			point += 3;
		}
		else if (clearSecondTime < 60 * 2) {	//�]��A
			point += 2;
		}
		else if (clearSecondTime < 60 * 3) {	//�]��B
			point += 1;
		}
		else {										//�]��C
			point += 0;
		}

		//�ō����m�x
		if (maxDetectionLevel < 0.25f) {				//�]��S
			point += 3;
		}
		else if (maxDetectionLevel < 0.5f) {			//�]��A
			point += 2;
		}
		else if (maxDetectionLevel < 0.75f) {		//�]��B
			point += 1;
		}
		else {										//�]��C
			point += 0;
		}

		const char* rank = nullptr;

		//��L�̃|�C���g���烉���N���o��
		if (point <= 1) {
			rank = "c";
		}
		else if (point <= 3) {
			rank = "b";
		}
		else if (point <= 5) {
			rank = "a";
		}
		else {
			rank = "s";
		}

		return rank;
	}
};