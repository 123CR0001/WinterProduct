#include"SoundComponent.h"
#include"ObjectBase.h"
#include"PhysWorld.h"
#include"ObjectServer.h"

SoundComponent::SoundComponent(ObjectBase* owner, float volumeSize)
	:Component(owner)
	, _volumeSize(volumeSize)
	,_pos(owner->GetPos())
{
	//physWorld‚É“o˜^
	_owner->GetObjectServer()->GetPhysWorld()->GetSoundComponent().emplace_back(this);
}

SoundComponent::SoundComponent(ObjectBase* owner,const Vector3& pos, float volumeSize)
	:Component(owner)
	, _volumeSize(volumeSize)
	, _pos(pos)
{	
	//physWorld‚É“o˜^
	_owner->GetObjectServer()->GetPhysWorld()->GetSoundComponent().emplace_back(this);
}

SoundComponent::~SoundComponent() {
	//physWorld‚Ì“o˜^‚ğíœ
	auto physWorld = _owner->GetObjectServer()->GetPhysWorld();

	for (auto iter = physWorld->GetSoundComponent().begin(); iter != physWorld->GetSoundComponent().end(); ++iter) {
		physWorld->GetSoundComponent().erase(iter);
		break;
	}
}

bool SoundComponent::Process() {
	//1ƒtƒŒ[ƒ€‚ÅÁ‚·
	_owner->DeleteComponent(this);
	return true;
}

bool SoundComponent::IsHear(const Vector3& pos) {
	//_volumeSize‚æ‚è‹——£‚ª’Z‚¯‚ê‚ÎAtrue‚ğ•Ô‚·
	if (Vector3::LengthSquare(pos, _pos) < _volumeSize * _volumeSize) {
		return true;
	}
	
	return false;
}
