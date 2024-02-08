#include"AfterImage.h"
#include"AnimationConmponent.h"
#include"ObjectServer.h"

AfterImage::AfterImage(AnimationComponent* anim)
	:ObjectBase(anim->GetOwner()->GetObjectServer())
	,_alpha(0.5f)
{
	//�c���𐶐�����n���h������n���h���𐶐�����
	_handle = MV1DuplicateModel(anim->GetOwner()->GetHandle());

	//���������n���h���ɁA�A�j���[�V������K��
	MV1AttachAnim(_handle, anim->GetAnimationAttachIndex());
	MV1SetAttachAnimTime(_handle, anim->GetAnimationAttachIndex(), anim->GetPlayTime());

	//�ʒu����
	_pos = anim->GetOwner()->GetPos();

	//�p�x����
	_eulerAngle = anim->GetOwner()->GetEulerAngle();

	//�X�P�[���l����
	_scale = anim->GetOwner()->GetScale();

	//�e�l�����f���ɓK��
	MV1SetPosition(_handle, GetDxPos());
	//���f����Y����180�x��]
	_eulerAngle.y += PI;
	MV1SetRotationXYZ(_handle, DxConverter::VecToDx(_eulerAngle));

	MV1SetScale(_handle, DxConverter::VecToDx(_scale));

}

AfterImage::~AfterImage() {

}

bool AfterImage::Process() {

	//�����x���㏸
	_alpha -= 0.01f;

	//���f���ɓ����x��K��
	MV1SetOpacityRate(_handle, _alpha);

	//�����x��0�ȉ��ɂȂ�����폜
	if (_alpha < 0.f) {
		GetObjectServer()->DeleteObject(this);
	}

	return true;
}

bool AfterImage::Render() {

	MV1DrawModel(_handle);
	return true;
}