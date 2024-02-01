#include"ModeLightsOut.h"
#include"ModeColorOut.h"
#include"ModeColorIn.h"

bool ModeLightsOut::Initialize() {

	std::vector<int>noiseCgs;
	noiseCgs.resize(6);
	ResourceServer::LoadDivGraph("res/Effect/ui_nightscope_1.png",
		6,
		1,
		6, 
		1920,
		1080,
		noiseCgs.data()
	);
	_noiseAnim = new Animation2D(
		noiseCgs,
		10,
		0, 0, ApplicationBase::GetInstance()->DispSizeW(), ApplicationBase::GetInstance()->DispSizeH()
	);

	_cg = ResourceServer::LoadGraph("res/Effect/nightscope_01.png");


	_alpha = 170;
	return true;
}

bool ModeLightsOut::Terminate() {

	return true;
}

bool ModeLightsOut::Process() {
	if (ApplicationBase::GetInstance()->GetPad()->GetTrgButton() & INPUT_Y && !ModeServer::GetInstance()->IsAdd("Out")) {
		ModeColorIn* colorIn = new ModeColorIn(10);
		ModeServer::GetInstance()->Add(new ModeColorOut(colorIn,this, 10), 10, "Out");
	}
	return true;
}

bool ModeLightsOut::Render() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);	// ”¼“§–¾ƒ‚[ƒh
	auto instance = ApplicationBase::GetInstance();
	DrawModiGraph(
		0, 0,
		instance->DispSizeW(),0,
		instance->DispSizeW(), instance->DispSizeH(),
		0, instance->DispSizeH(),
		_cg,
		TRUE
	);
	_noiseAnim->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alpha);

	return true;
}