
#include "UIResume.h"
#include "UIServer.h"

UIResume::UIResume(float initPosX, float initPosY, float endPosX, float endPosY, int frame)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame)
{
}

int UIResume::Selected() {
    // UIServer���i�[���Ă��邩
    if (typeid(UIServer*) != typeid(static_cast<UIServer*>(_param))) { return -1; }
    UIServer* server = static_cast<UIServer*>(_param);

    // �T�[�o�[����UI���������āA�X���C�h������
    std::vector<std::string> targetNames = { "system", "left_frame", "right_frame", "control", "audio", "resume", "to_title", "get" };
    for (auto& name : targetNames) {
        auto ui = server->Search(name);
        if (ui) {
            float initX = ui->_endPosX;
            float initY = ui->_endPosY;
            float endX = ui->_initPosX;
            float endY = ui->_initPosY;
            int frame = ui->_frame;
            // �X���C�h������
            ui->SetSlideUIPosition(initX, initY, endX, endY, frame);
        }
    }

	return 1;
}
