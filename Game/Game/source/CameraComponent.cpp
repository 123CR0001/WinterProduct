#include"CameraComponent.h"
#include"ObjectBase.h"
CameraComponent::CameraComponent(ObjectBase* owner, int order)
	:Component(owner, order) {

}

CameraComponent::~CameraComponent(){}

bool CameraComponent::Process() {
	if (_swapWidth != 0 || _swapHeight != 0) {
		int w = _swapWidth;
		int h = _swapHeight;

		if (rand() % 2 == 0) {	w *= -1; }
		if (rand() % 2 == 0) { h *= -1; }

		SetCameraScreenCenter(
			ApplicationBase::GetInstance()->DispSizeW() / 2 + w,
			ApplicationBase::GetInstance()->DispSizeH() / 2 + h
		);

		if (_swapWidth > 0) { _swapWidth -= rand() % 5; }
		else { _swapWidth = 0; }
		if (_swapHeight > 0) { _swapHeight -= rand() % 5; }
		else { _swapHeight = 0; }
	}


	return true;
}