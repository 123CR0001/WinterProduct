#include"CameraZoomComponent.h"
#include"CameraComponent.h"

#include"ObjectBase.h"

#include"appframe.h"

CameraZoomComponent::CameraZoomComponent(CameraComponent* camera, float changeMag, int maxFrameCnt)
	:Component(camera->GetOwner())
	,_camera(camera)
	, _changeMag(changeMag)
	, _maxFrameCnt(maxFrameCnt)
	,_mag(camera->GetTargetDistMag())
	,_frameCnt(0)
{

}

CameraZoomComponent::~CameraZoomComponent(){}

bool CameraZoomComponent::Process() {

	//ズームが終わったら、削除
	if(_frameCnt == _maxFrameCnt) { _owner->DeleteComponent(this); }
	
	//ズーム
	_camera->SetTargetDistmag(EasingLinear(static_cast<float>(_frameCnt), _mag, _changeMag, static_cast<float>(_maxFrameCnt)));

	//カウントを増やす
	_frameCnt++;

	return true;
}