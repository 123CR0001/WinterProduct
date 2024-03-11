#include"UIDetectionLevel.h"
#include"SpriteText.h"
#include"ObjectServer.h"
#include"AIComponent.h"
#include"CommonSoldier.h"
#include"Player.h"

UIDetectionLevel::UIDetectionLevel(ObjectServer* server)
	:UI()
	,_handle(LoadGraph("res/UI/Game/cir.png"))
	,_text(NEW SpriteText())
	,_server(server)
{

}

UIDetectionLevel::~UIDetectionLevel(){}

bool UIDetectionLevel::Process() {

	return true;
}

bool UIDetectionLevel::Draw() {

	auto player = _server->GetPlayer();

	for(auto iter = _server->GetCommonSoldiers().begin(); iter != _server->GetCommonSoldiers().end(); ++iter) {
		if((*iter)->GetAIComponent()->IsFound(player)) {

			VECTOR pos = ConvWorldPosToScreenPos(DxConverter::VecToDx((*iter)->GetPos() + Vector3D(50.f, 170.f, 0.f)));

			double level = static_cast<double>((*iter)->GetDetectionLevel() * 100.f);

			DrawCircleGaugeF(pos.x, pos.y, level, _handle);

		}
	}

	return true;
}