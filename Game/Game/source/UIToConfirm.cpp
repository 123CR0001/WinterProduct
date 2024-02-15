
#include "ModePause.h"
#include "ModePauseConfirm.h"

#include "UIToConfirm.h"

UIToConfirm::UIToConfirm(float initPosX, float initPosY, float endPosX, float endPosY, int frame)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame)
{
}

int UIToConfirm::Selected() {
	// �m�F��ʂ�\��
    ModeServer::GetInstance()->Add(new ModePauseConfirm(), 700, "pause_confirm");
    
	return 0;
}
