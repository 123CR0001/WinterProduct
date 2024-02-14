
#include "UIServer.h"
#include "UISelectController.h"
#include "UISelectMap.h"

UISelectController::UISelectController(float initPosX, float initPosY, float endPosX, float endPosY, int frame)
	:UISlide(initPosX, initPosY, endPosX, endPosY, frame) {
    bSelected = true;
}

int UISelectController::Selected() {
    std::string stage = _uiName;
    int stageNum = std::stoi(_uiName.substr(5));
    //auto server = GetUIServer();
  
    if (typeid(UIServer*) != typeid(static_cast<UIServer*>(_param))) { return -1; }
    UIServer* server = static_cast<UIServer*>(_param);

    std::vector<float> vStagePos = { 192, 320, 448, 925 };

    std::vector<float> vAreaPos = { 320, 448, 576, 704 };

    if (bSelected) {
        bSelected = false;
        // UIの位置設定
        int i = 0;
        for (auto& ui : server->_vUI) {
            if (ui->_uiName.substr(0, 4) == stage.substr(0, 4) || ui->_uiName == "toTitle") {
                ui->SetSlideUIPosition(ui->_x, ui->_y, -770, vStagePos[i], 5);
                ui->_selectNum = -1;
                i++;
            }
        }

        // ステージに関連するUIの登録済みチェックと再利用
        for (int i = 1; i <= 4; ++i) {
            std::string uiName = "stage" + std::to_string(stageNum) + "_" + std::to_string(i);
            if (server->IsRegistering(uiName)) {
                server->Search(uiName)->SetSlideUIPosition(-672, vAreaPos[i - 1], 128, vAreaPos[i - 1], 10);
                server->Search(uiName)->_selectNum = i;
            }
            else {
                std::string path = "res/UI/StageSelect/stage" + std::to_string(stageNum) + "/ui_stage0" + std::to_string(stageNum) + "_" + std::to_string(i) + ".png";
                int cgStage = res::LoadGraph(path.c_str());
                server->Add(new UISelectMap(-672, vAreaPos[i - 1], 128, vAreaPos[i - 1], 10, std::to_string(i)),
                    nullptr, cgStage, -672, vAreaPos[i - 1], 600, 100, BASIC_LAYER_VALUE, uiName);
                server->Search(uiName)->_selectNum = i;
            }
        }

        // 対象のステージに関連するUIの位置設定
        for (auto& ui : server->_vUI) {
            if (ui->_uiName == stage) {
                ui->SetSlideUIPosition(ui->_x, ui->_y, 30, 192, 5);
                ui->_selectNum = 0;
            }
        }
    }
    else {
        bSelected = true;
        // UIの位置設定
        for (int i = 1; i <= 4; ++i) {
            std::string uiName = "stage" + std::to_string(stageNum) + "_" + std::to_string(i);
            server->Search(uiName)->SetSlideUIPosition(128, vAreaPos[i - 1], -672, vAreaPos[i - 1],  5);
            server->Search(uiName)->_selectNum = -1;
        }

        // 対象のステージに関連するUIの位置設定
        for (auto& ui : server->_vUI) {
            if (ui->_uiName == stage) {
                int selectNum = std::stoi(ui->_uiName.substr(5));
                ui->SetSlideUIPosition(ui->_x, ui->_y, 30, vStagePos[selectNum - 1], 5);
                ui->_selectNum = stageNum - 1;
                break;
            }
        }

        // UIの位置設定
        for (auto& ui : server->_vUI) {
            if (ui->_uiName == "toTitle") {
                ui->SetSlideUIPosition(ui->_x, ui->_y, 30, vStagePos[3], 5);
                ui->_selectNum = 3;
            }
            if (ui->_uiName.size() > 6) { continue; }
            if (ui->_uiName.substr(0, 4) == stage.substr(0, 4)) {
                int selectNum = std::stoi(ui->_uiName.substr(5));
                ui->SetSlideUIPosition(ui->_x, ui->_y, 30, vStagePos[selectNum - 1], 5);
                ui->_selectNum = std::stoi(ui->_uiName.substr(5)) - 1;
            }
        }
    }

	return 0;
}
