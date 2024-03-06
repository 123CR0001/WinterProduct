#include"CameraZoomComponent.h"
#include"CameraComponent.h"

#include"ObjectBase.h"

#include"appframe.h"

CameraZoomComponent::CameraZoomComponent(CameraComponent* camera, const float changeMag, const int maxFrameCnt)
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

	//�Y�[�����I�������A�폜
	if(_frameCnt == _maxFrameCnt) { _owner->DeleteComponent(this); }
	
	//�Y�[��
	_camera->SetTargetDistmag(EasingLinear(static_cast<float>(_frameCnt), _mag, _mag + _changeMag, static_cast<float>(_maxFrameCnt)));

	//�J�E���g�𑝂₷
	_frameCnt++;

	return true;
}