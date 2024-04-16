#pragma once
#include"UI.h"

class UIDetectionLevel :public UI {
public:
	UIDetectionLevel(class ObjectServer* server,int drawOrder = 100);
	~UIDetectionLevel();

	const char* GetName()override { return "DetectionLavel"; }

	bool Process()override;
	bool Draw()override;

private:

	int _gaugeHandle;				//�Q�[�W�̃n���h��
	int _gaugeBgHandle;				//�Q�[�W�̔w�i�̃n���h��
	class ObjectServer* _server;	//�I�u�W�F�N�g�T�[�o�[
};