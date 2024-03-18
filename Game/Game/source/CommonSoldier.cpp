#include"CommonSoldier.h"
#include"Player.h"
#include"ObjectServer.h"

#include<algorithm>

#include"PhysWorld.h"

#include"MoveComponent.h"
#include"CapsuleComponent.h"
#include"FrameComponent.h"
#include"CameraComponent.h"
#include"CameraZoomComponent.h"
#include"MotionComponent.h"

#include"ModeGame.h"
#include"ModeEffekseer.h"

#include"AIComponent.h"
#include"AIPatrol.h"
#include"AIBackPatrol.h"
#include"AIMoveTo.h"
#include"AIBlindWalk.h"
#include"AIPanic.h"
#include"AILookAround.h"
#include"AIDeath.h"
#include"AIStay.h"
#include"AIDiscovery.h"
#include"AIAttack.h"
#include"AILoseSight.h"

#include"CommonSoldierAnimaitonComponent.h"
#include"CountKillComboComponent.h"
#include"CameraZoomComponent.h"

#include"LightsOut.h"
#include"TimeLine.h"

#include"MyUIServer.h"
#include"UI.h"

#include"Gun.h"

CommonSoldier::CommonSoldier(ObjectServer* server) 
	:CharaBase(server,"CommonSoldier")
	, _AI(NEW AIComponent(this, 11))
	,_capsule(NEW CapsuleComponent(this,1000))
	,_anim(NEW CommonSoldierAnimationComponent(this,1000))
	,_detectionLevel(0.f)
	,_isAttack(false)
	,_gun(NEW Gun(this))
{
	NEW CountKillComboComponent(this);

	//AState�̓o�^
	_AI->RegisterState(NEW AIBackPatrol(_AI));
	_AI->RegisterState(NEW AIPatrol(_AI));
	_AI->RegisterState(NEW AIBlindWalk(_AI));
	_AI->RegisterState(NEW AIMoveTo(_AI));
	_AI->RegisterState(NEW AIPanic(_AI));
	_AI->RegisterState(NEW AILookAround(_AI));
	_AI->RegisterState(NEW AIDeath(_AI));
	_AI->RegisterState(NEW AIStay(_AI));
	_AI->RegisterState(NEW AIDiscovery(_AI));
	_AI->RegisterState(NEW AIAttack(_AI));
	_AI->RegisterState(NEW AILoseSight(_AI));

	server->GetCommonSoldiers().emplace_back(this);
	server->GetEnemys().emplace_back(this);

	_capsule->AddSkipName("CommonSoldier");

	auto motion = NEW MotionComponent(_anim, 10000);

	{
		auto func = [=](const MOTION_DATA_ITEM&) {
			_gun->OnAttack();
			motion->IncrementMotionCount();
		};

		motion->RegisterCustomCommand("ATTACK_ON", func);
	}

	server->GetGame()->IncrementEnemyCount();

	//�ŏ��͗֊s����`�悵�Ȃ�
	for (int a = 0; a < MV1GetMaterialNum(_handle); a++) {
		MV1SetMaterialOutLineWidth(_handle, a, 0.f);
	}

}

CommonSoldier::~CommonSoldier(){
}

bool CommonSoldier::Initialize() {
	ObjectBase::Initialize();

	// 3D���f����1�Ԗڂ̃A�j���[�V�������A�^�b�`����
	_attachIndex = 0;
	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����


	//�J�v�Z���̃����o��ݒ�
	_capsule->SetMember(180.f, 30.f);

	//AI�̎��o�̃����o��ݒ�
	_AI->SetViewAngle(120.f);
	_AI->SetViewDist(500.f);
	_AI->SetView(Vector3(0.f, 100.f, 0.f));

	_detectionLevel = 0.f;

	//�ŏ���AI���
	_AI->ChangeState("Patrol");

	_capsule->AddSkipName("Decoy");



	return true;
}

bool CommonSoldier::Terminate() {
	CharaBase::Terminate();



	auto iter = std::find(GetObjectServer()->GetCommonSoldiers().begin(), GetObjectServer()->GetCommonSoldiers().end(), this);
	if (iter != GetObjectServer()->GetCommonSoldiers().end()) {
		GetObjectServer()->GetCommonSoldiers().erase(iter);
	}

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
	if (_AI->IsFound(GetObjectServer()->GetPlayer()) && GetObjectServer()->GetGame()->GetLightsOut()->IsUse()) {
		//AI���Ƃɏ㏸���邩���Ȃ����𕪂��邩������Ȃ�
		//�����ɏ����Ă���̂͑��菑��
		const float dist = _AI->GetViewDist();

		float per = 100.f;// Vector3D::LengthSquare(GetObjectServer()->GetPlayer()->GetPos(), _pos) / dist * dist;
		per = -(per - 1);

		//���m�x�̏㏸
		_detectionLevel += 1.f * 0.01;
	}
	else {
		_detectionLevel -= 0.01f;
	}

	//���m�x��100%�ɂȂ���
	if(_detectionLevel >= 1.f && !_isAttack) {
		{
			//���m�x��UI���폜
			auto ui = GetObjectServer()->GetGame()->GetUIServer()->Get("DetectionLavel");

			if(ui) {
				GetObjectServer()->GetGame()->GetUIServer()->DeleteUI(ui);
			}
		}

		_AI->ChangeState("Attack");

		NEW CameraZoomComponent(GetObjectServer()->GetPlayer()->GetCamera(), 0.6f, 60);

		//�U������
		_isAttack = true;

		//1�t���[����ɁA�L�����N�^�[���S
		auto timeLine = GetObjectServer()->GetGame()->GetTimeLine();
		timeLine->AddLine(
			1, 
			[=]()mutable {
				GetObjectServer()->GetPlayer()->ApplyDamage(
					DamageData{
						true,this,PhysWorld::CollisionDetectionItem{}
					}
				);
			}
		);
	}

	_detectionLevel = Clamp(0.f, 1.f, _detectionLevel);


	//�_���[�W���󂯂���
	if (_damageData.isDamage) {

		//���S
		_AI->ChangeState("Death");

		auto iter2 = std::find(GetObjectServer()->GetEnemys().begin(), GetObjectServer()->GetEnemys().end(), this);
	
		if (iter2 != GetObjectServer()->GetEnemys().end()){
			GetObjectServer()->GetEnemys().erase(iter2);
		}

		//�G�t�F�N�g�Đ�
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood01",
			_damageData.item.hitPosition,
			Vector3(0.f, atan2f(_damageData.item.pushVec.x, _damageData.item.pushVec.z), 0.f)
		);
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood02",
			_pos + Vector3(0.f, 1.f, 0.f),
			_eulerAngle
		);
		//�J�����̗h��
		GetObjectServer()->GetPlayer()->GetCamera()->Swap();

		_state = STATE::kDead;

		DeleteComponent(_capsule);

		//_enemyCout�����炷
		GetObjectServer()->GetGame()->DecrementEnemyCount();

		//�v���C���[�̈ړ��X�s�[�h�̔{�����グ��
		GetObjectServer()->GetPlayer()->AddMoveSpeedMag(0.2f);

		//���C�c�A�E�g�̐������Ԃ�L�΂�
		GetObjectServer()->GetGame()->GetLightsOut()->AddFrameCount();

		//�f�[�^����ɂ���
		_damageData = DamageData{};
	}

	
	if(GetObjectServer()->GetGame()->GetResultData()->maxDetectionLevel < _detectionLevel){
		GetObjectServer()->GetGame()->GetResultData()->maxDetectionLevel = _detectionLevel;
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

	//AIPatrol�̏���o�H��o�^
	for (auto&& marker : data) {

		std::string name = marker.at("objectName");
		//�I�u�W�F�N�g���{����		��FEnemy1
		//�����������폜

		//0~9�̐������܂܂�Ă���΁A����ȍ~�̕������폜
		for(int a = 0; a < 10; a++) {
			//������string�ɕϊ��@����������
			int num = name.find(std::to_string(a));

			//find�́A���������������Ȃ���΁A-1��Ԃ�
			if(num != -1) { name = name.substr(0, num); break; }
		}

		const Vector3 pos(marker.at("translate").at("x"), marker.at("translate").at("z"), -1 * marker.at("translate").at("y"));
		_AI->AddPoint(state->GetName(), pos);
		
	}
	if (_AI->GetPoints(state->GetName()).size() > 0) {
		SetPos(_AI->GetPoints(state->GetName()).front());
		const Vector3 deg = Vector3(data.front().at("rotate").at("x"), data.front().at("rotate").at("z"), data.front().at("rotate").at("y"));
		SetEulerAngleDeg(deg);
	}

	ModelMatrixSetUp();

}