#include"CommonSoldier.h"
#include"Player.h"
#include"ObjectServer.h"

#include<algorithm>

#include"PhysWorld.h"

#include"MoveComponent.h"
#include"CapsuleComponent.h"
#include"FrameComponent.h"
#include"CameraComponent.h"

#include"ModeGame.h"
#include"ModeEffekseer.h"

#include"AIComponent.h"
#include"AIPatrol.h"
#include"AIChase.h"
#include"AIBackPatrol.h"
#include"AICheckPoint.h"
#include"AIBlindWalk.h"
#include"AIPanic.h"
#include"AILookAround.h"
#include"AIDeath.h"
#include"AIStay.h"
#include"AIStare.h"

#include"CommonSoldierAnimaitonComponent.h"
#include"CountKillComboComponent.h"

constexpr int SIDE_NUM = 100;

CommonSoldier::CommonSoldier(ObjectServer* server) 
	:CharaBase(server,"CommonSoldier")
	, _AI(NEW AIComponent(this, 11))
	,_capsule(NEW CapsuleComponent(this,1000))
	,_anim(NEW CommonSoldierAnimationComponent(this,1000))
	,_detectionLevel(0.f)
{
	NEW CountKillComboComponent(this);

	//AState�̓o�^
	_AI->RegisterState(NEW AIBackPatrol(_AI));
	_AI->RegisterState(NEW AIChase(_AI));
	_AI->RegisterState(NEW AIPatrol(_AI));
	_AI->RegisterState(NEW AIBlindWalk(_AI));
	_AI->RegisterState(NEW AICheckPoint(_AI));
	_AI->RegisterState(NEW AIPanic(_AI));
	_AI->RegisterState(NEW AILookAround(_AI));
	_AI->RegisterState(NEW AIDeath(_AI));
	_AI->RegisterState(NEW AIStay(_AI));
	_AI->RegisterState(NEW AIStare(_AI));

	server->GetCommonSoldiers().emplace_back(this);
	server->GetEnemys().emplace_back(this);

	for(int a = 0; a < SIDE_NUM; a++) {
		_versNums.emplace_back(0);
		_versNums.emplace_back(a + 1);
		_versNums.emplace_back(a + 2);
	}

	_visionHandleRed = ResourceServer::LoadGraph("res/UI/visualrange_01.png");
	_visionHandleBlue = ResourceServer::LoadGraph("res/UI/visualrange_02.png");

	_capsule->AddSkipName("CommonSoldier");
}

CommonSoldier::~CommonSoldier(){
}

bool CommonSoldier::Initialize() {
	ObjectBase::Initialize();

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
	if (_AI->IsFound(GetObjectServer()->GetPlayer())) {
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
			Vector3D(0.f, atan2f(_damageData.item.pushVec.x, _damageData.item.pushVec.z), 0.f)
		);
		GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
			"Blood02",
			_pos + Vector3D(0.f, 10.f, 0.f),
			_eulerAngle
		);
		//�J�����̗h��
		GetObjectServer()->GetPlayer()->GetCamera()->Swap();

		_state = STATE::kDead;

		DeleteComponent(_capsule);

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

	//�L�����N�^�[���ނ��Ă���p�x����A
	float rot = _eulerAngle.y - DegToRad(_AI->GetViewAngle()) / 2.f;

	//�O�t���[���̒��_�f�[�^���폜
	_vers.clear();

	//���Əd�Ȃ�Ȃ��悤�ɁA������肿����Ə�Ɉʒu�𒲐�
	Vector3D pos = _pos + Vector3D(0.f, 1.f, 0.f);

	int handle = _visionHandleRed;
	COLOR_U8 dif = GetColorU8(255, 0, 0, 0);
	COLOR_U8 spc = GetColorU8(255, 0, 0, 0);

	if(_AI->IsFound(GetObjectServer()->GetPlayer())) {
		handle = _visionHandleBlue;
		dif = GetColorU8(0, 0, 255, 0);
		spc = GetColorU8(0, 0, 255, 0);
	}

	//���S�_
	_vers.emplace_back(
		VERTEX3D{
			DxConverter::VecToDx(Vector3D(pos)),
			VGet(0.f,1.f,0.f),
			dif,
			spc,
			0.5f,
			0.5f,
			0.f,
			0.f
		}
	);

	for(int a = 0; a <= SIDE_NUM; a++) {

		Vector3D viewPos = pos + Vector3D(sinf(rot), 0.f, cosf(rot) ) * _AI->GetViewDist();

		for(auto&& frame : GetObjectServer()->GetPhysWorld()->GetFrameComponent()) {
			MV1_COLL_RESULT_POLY result = 
				MV1CollCheck_Line(
					frame->GetOwner()->GetHandle(),
					frame->GetOwner()->GetAttachIndex(),
					DxConverter::VecToDx(pos),
					DxConverter::VecToDx(viewPos)
				);

			if(result.HitFlag) { viewPos = DxConverter::DxToVec(result.HitPosition); }
		}

		VERTEX3D ver = {
			DxConverter::VecToDx(viewPos),
			VGet(0.f,1.f,0.f),
			dif,
			spc,
			0.5f + 0.5f * (Vector3D::Length(viewPos,pos) / _AI->GetViewDist()),
			0.5f + 0.5f * (Vector3D::Length(viewPos,pos) / _AI->GetViewDist()),
			0.f,
			0.f
		};

		_vers.emplace_back(ver);

		rot += DegToRad(_AI->GetViewAngle()) / SIDE_NUM;

	}


	SetUseLighting(FALSE);
	DrawPolygonIndexed3D(_vers.data(), _vers.size(), _versNums.data(), _versNums.size() / 3, handle,FALSE);
	SetUseLighting(TRUE);

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

		const Vector3D pos(marker.at("translate").at("x"), marker.at("translate").at("z"), -1 * marker.at("translate").at("y"));
		_AI->AddPoint(state->GetName(), pos);
		
	}
	if (_AI->GetPoints(state->GetName()).size() > 0) {
		SetPos(_AI->GetPoints(state->GetName()).front());
		const Vector3D deg = Vector3D(data.front().at("rotate").at("x"), data.front().at("rotate").at("z"), data.front().at("rotate").at("y"));
		SetEulerAngleDeg(deg);
	}

	ModelMatrixSetUp();

}