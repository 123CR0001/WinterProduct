#pragma once
#include<string>
	//�N���A��ʂ̃��U���g�f�[�^
class ResultData {
public:
	ResultData() {
		maxCombo = 0;
		clearSecondTime = 0.f;
		maxDetectionLevel = 0.f;
	}
	~ResultData() {}

	//�ō��R���{��
	int maxCombo;

	//�N���A�^�C��
	float clearSecondTime;

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
		if (clearSecondTime < 60.f * 1.f) {			//�]��S
			point += 3;
		}
		else if (clearSecondTime < 60.f * 2.f) {	//�]��A
			point += 2;
		}
		else if (clearSecondTime < 60.f * 3.f) {	//�]��B
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
