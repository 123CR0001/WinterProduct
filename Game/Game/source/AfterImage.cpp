#include"AfterImage.h"
#include"AnimationConmponent.h"
#include"ObjectServer.h"

AfterImage::AfterImage(AnimationComponent* anim)
	:ObjectBase(anim->GetOwner()->GetObjectServer())
	,_alpha(0.5f)
{
	//残像を生成するハンドルからハンドルを生成する
	_handle = MV1DuplicateModel(anim->GetOwner()->GetHandle());

	//生成したハンドルに、アニメーションを適応
	MV1AttachAnim(_handle, anim->GetAnimationAttachIndex());
	MV1SetAttachAnimTime(_handle, anim->GetAnimationAttachIndex(), anim->GetPlayTime());

	//位置を代入
	_pos = anim->GetOwner()->GetPos();

	//角度を代入
	_eulerAngle = anim->GetOwner()->GetEulerAngle();

	//スケール値を代入
	_scale = anim->GetOwner()->GetScale();

	//各値をモデルに適応
	MV1SetPosition(_handle, GetDxPos());
	//モデルのY軸を180度回転
	_eulerAngle.y += PI;
	MV1SetRotationXYZ(_handle, DxConverter::VecToDx(_eulerAngle));

	MV1SetScale(_handle, DxConverter::VecToDx(_scale));

}

AfterImage::~AfterImage() {

}

bool AfterImage::Process() {

	//透明度を上昇
	_alpha -= 0.01f;

	//モデルに透明度を適応
	MV1SetOpacityRate(_handle, _alpha);

	//透明度が0以下になったら削除
	if (_alpha < 0.f) {
		GetObjectServer()->DeleteObject(this);
	}

	return true;
}

bool AfterImage::Render() {

	MV1DrawModel(_handle);
	return true;
}