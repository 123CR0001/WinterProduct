#pragma once
#include"UI.h"
#include<vector>
#include"Dxlib.h"

class UIVision :public UI {
public:
	UIVision(class ObjectServer* server, int drawOrder = 100);
	~UIVision();

	const char* GetName()override { return "Vision"; }

	bool Process()override;
	bool Draw()override;

private:

	int _handle;								//�摜�n���h��

	std::vector<unsigned short>_versNums;		//���_�ԍ�
	std::vector<VERTEX3D>_vers;					//���_���

	class ObjectServer* _server;				//�I�u�W�F�N�g�T�[�o�[

};