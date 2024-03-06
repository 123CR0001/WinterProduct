#include"SoundComponent.h"
#include"ObjectBase.h"
#include"PhysWorld.h"
#include"ObjectServer.h"

SoundComponent::SoundComponent(ObjectBase* owner, float volumeSize)
	:Component(owner)
	, _volumeSize(volumeSize)
	,_pos(owner->GetPos())
{
	//physWorld�ɓo�^
	_owner->GetObjectServer()->GetPhysWorld()->GetSoundComponent().emplace_back(this);
}

SoundComponent::SoundComponent(ObjectBase* owner,const Vector3D& pos, float volumeSize)
	:Component(owner)
	, _volumeSize(volumeSize)
	, _pos(pos)
{	
	//physWorld�ɓo�^
	_owner->GetObjectServer()->GetPhysWorld()->GetSoundComponent().emplace_back(this);
}

SoundComponent::~SoundComponent() {
	//physWorld�̓o�^���폜
	auto physWorld = _owner->GetObjectServer()->GetPhysWorld();

	for (auto iter = physWorld->GetSoundComponent().begin(); iter != physWorld->GetSoundComponent().end(); ++iter) {
		physWorld->GetSoundComponent().erase(iter);
		break;
	}
}

bool SoundComponent::Process() {
	//1�t���[���ŏ���
	_owner->DeleteComponent(this);
	return true;
}

bool SoundComponent::IsHear(const Vector3D& pos) {
	//_volumeSize��苗�����Z����΁Atrue��Ԃ�
	if (Vector3D::LengthSquare(pos, _pos) < _volumeSize * _volumeSize) {
		return true;
	}
	
	return false;
}
