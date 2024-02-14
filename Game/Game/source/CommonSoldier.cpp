#include"CommonSoldier.h"
#include"Player.h"
#include"ObjectServer.h"
#include"AIComponent.h"
#include"AIPatrol.h"
#include"AIChase.h"
#include"AIBackPatrol.h"
#include<algorithm>
#include"MoveComponent.h"
#include"AIBlindWalk.h"
#include"CapsuleComponent.h"
#include"FrameComponent.h"
#include"PhysWorld.h"
#include"AICheckPoint.h"
#include"AIBlindWalk.h"
#include"CameraComponent.h"
#include"ModeGame.h"
#include"ModeEffekseer.h"
#include"AIPanic.h"

CommonSoldier::CommonSoldier(ObjectServer* server) 
	:CharaBase(server,"CommonSoldier")
	, _AI(NEW AIComponent(this, 1))
	,_capsule(NEW CapsuleComponent(this,1000))
	,_detectionLevel(0.f)
{

	//AState�̓o�^
	_AI->RegisterState(NEW AIBackPatrol(_AI));
	_AI->RegisterState(NEW AIChase(_AI));
	_AI->RegisterState(NEW AIPatrol(_AI));
	_AI->RegisterState(NEW AIBlindWalk(_AI));
	_AI->RegisterState(NEW AICheckPoint(_AI));
	_AI->RegisterState(NEW AIBlindWalk(_AI));
	_AI->RegisterState(NEW AIPanic(_AI));

	server->GetCommonSoldiers().emplace_back(this);
	server->GetEnemys().emplace_back(this);
}

CommonSoldier::~CommonSoldier(){
	Terminate();
}

bool CommonSoldier::Initialize() {
	ObjectBase::Initialize();

	LoadModel("res/SDChar/SDChar.mv1");
	// 3D���f����1�Ԗڂ̃A�j���[�V�������A�^�b�`����
	_attachIndex = 0;
	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����

	_isStand = true;

	//�J�v�Z���̃����o��ݒ�
	_capsule->SetMember(180.f, 30.f);

	//AI�̎��o�̃����o��ݒ�
	_AI->SetViewAngle(120.f);
	_AI->SetViewDist(500.f);
	_AI->SetView(Vector3D(0.f, 100.f, 0.f));

	_detectionLevel = 0.f;

	//�ŏ���AI���
	_AI->ChangeState("Patrol");

	return true;
}

bool CommonSoldier::Terminate() {
	CharaBase::Terminate();
	auto iter = std::find(GetObjectServer()->GetCommonSoldiers().begin(), GetObjectServer()->GetCommonSoldiers().end(), this);
	GetObjectServer()->GetCommonSoldiers().erase(iter);

	auto iter2 = std::find(GetObjectServer()->GetEnemys().begin(), GetObjectServer()->GetEnemys().end(), this);
	GetObjectServer()->GetEnemys().erase(iter2);

	return true;
}

bool CommonSoldier::Process() {

	ObjectBase::Process();

	//�p�x�̏C��
	if (_eulerAngle.y > 2 * PI) {
		_eulerAngle.y = 0;
	}
	if (_eulerAngle.y <  0) {
		_eulerAngle.y = 2 * PI;
	}

	//���m�x�̑���(�S����)
	if (_AI->IsFound(GetObjectServer()->GetPlayer())) {
		//AI���Ƃɏ㏸���邩���Ȃ����𕪂��邩������Ȃ�
		//�����ɏ����Ă���̂͑��菑��
		const float dist = _AI->GetViewDist();

		float per = Vector3D::LengthSquare(GetObjectServer()->GetPlayer()->GetPos(), _pos) / dist * dist;
		per = -(per - 1);

		//���m�x�̏㏸
		_detectionLevel += per * 0.01;
	}
	else {
		_detectionLevel -= 0.01f;
	}
	_detectionLevel = Clamp(0.f, 1.f, _detectionLevel);


	//�_���[�W���󂯂���
	if (_damageData.isDamage) {

		//�������폜�o�^
		GetObjectServer()->DeleteObject(this);

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

		//�f�[�^����ɂ���
		_damageData = DamageData{};
	}

	return true;
}

bool CommonSoldier::Render() {
	CharaBase::Render();
	return true;
}

void CommonSoldier::SetJsonDataUE(nlohmann::json data) {

	AIState* state = _AI->GetStateMap("Patrol");

	if (!state) { return; }

	char num[10];
	int count = 1;

	//AIPatrol�̏���o�H��o�^
	for (auto&& marker : data) {

		std::string name = "marker";
		snprintf(num, 8, "%d", count);
		name += num;
		count++;

		if (marker.at("objectName") == name) {
			Vector3D pos(marker.at("translate").at("x"), marker.at("translate").at("z"), -1 * marker.at("translate").at("y"));
			_AI->AddPoint(state->GetName(), pos);
		}
	}
	if (_AI->GetPoints(state->GetName()).size() > 0) {
		SetPos(_AI->GetPoints(state->GetName()).front());
	}

	ModelMatrixSetUp();
}