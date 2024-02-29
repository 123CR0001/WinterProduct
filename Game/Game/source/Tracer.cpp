#include"Tracer.h"
#include"ObjectServer.h"

#include"AIComponent.h"
#include"AITrace.h"
#include"AIRush.h"

#include"MoveComponent.h"
#include"AnimationComponent.h"
#include"CapsuleComponent.h"

#include"Player.h"

#include"TracerSpawner.h"

#include"ModeGame.h"
#include"ModeEffekseer.h"
#include"CameraComponent.h"
#include"TracerAnimationComponent.h"

Tracer::Tracer(TracerSpawner* spawner)
	:CharaBase(spawner->GetObjectServer(), "Trace")
	, _capsule(NEW CapsuleComponent(this, 1000))
	,_AI(NEW AIComponent(this,1))
	,_anim(NEW TracerAnimationComponent(this,1000))
	,_frameCnt(0)
	,_maxFrame(20)
{

	_attachIndex =  MV1SearchFrame(_handle, "Soldier");

	//AIState��o�^
	_AI->RegisterState(NEW AITrace(_AI));
	_AI->RegisterState(NEW AIRush(_AI));

	_AI->ChangeState("Trace");

	//���܂ŋL�^���Ă����A�v���C���[�̍��W��move
	_AI->GetPoints("Trace") = std::move(spawner->GetPoints());

	//�ŏ��̍��W�Ɉړ�
	_pos = _AI->GetPoints("Trace").front();

	GetObjectServer()->GetEnemys().emplace_back(this);

}

Tracer::~Tracer(){

}

bool Tracer::Initialize() {

	//�J�v�Z���̃����o��ݒ�
	_capsule->SetMember(180.f, 30.f);

	//AI�̎��o�̃����o��ݒ�
	_AI->SetViewAngle(120.f);
	_AI->SetViewDist(500.f);
	_AI->SetView(Vector3D(0.f, 100.f, 0.f));

	return true;
}

bool Tracer::Terminate() {
	CharaBase::Terminate();
	return true;
}

bool Tracer::Process() {
	ObjectBase::Process();

	if (_frameCnt == _maxFrame) {

		_frameCnt = 0;

		_AI->AddPoint("Trace", GetObjectServer()->GetPlayer()->GetPos());
	}

	_frameCnt++;

	//�_���[�W���󂯂���
	if (_damageData.isDamage) {

		//���S
		_AI->ChangeState("Death");

		auto iter2 = std::find(GetObjectServer()->GetEnemys().begin(), GetObjectServer()->GetEnemys().end(), this);

		if (iter2 != GetObjectServer()->GetEnemys().end()) {
			GetObjectServer()->GetEnemys().erase(iter2);
		}

		//�G�t�F�N�g�Đ�
		//GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
		//	"Blood01",
		//	_damageData.item.hitPosition,
		//	Vector3D(0.f, atan2f(_damageData.item.pushVec.x, _damageData.item.pushVec.z), 0.f)
		//);
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood02",
			_pos + Vector3D(0.f, 10.f, 0.f),
			_eulerAngle
		);
		//�J�����̗h��
		GetObjectServer()->GetPlayer()->GetCamera()->Swap();

		DeleteComponent(_capsule);

		//�f�[�^����ɂ���
		_damageData = DamageData{};
	}

	return true;
}