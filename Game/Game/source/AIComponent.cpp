#include"AIComponent.h"
#include"AIState.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"PhysWorld.h"
#include"FrameComponent.h"

AIComponent::AIComponent(ObjectBase* owner,int order)
	:Component(owner,order)
	,_currentState(nullptr)
{
}

AIComponent::~AIComponent(){
	for (auto&& state : _stateMap) {
		delete state.second;
	}

	_stateMap.clear();
}

bool AIComponent::Process() {

	if (_currentState) {
		_currentState->Process();
	}
	return true;
}

bool AIComponent::ChangeState(const std::string& name) {
	if (_currentState) {
		_currentState->OnExist();
	}

	auto iter = _stateMap.find(name);

	if (iter != _stateMap.end()) {
		_currentState = iter->second;
		_currentState->OnEnter();
		return true;
	}
	else { _currentState = nullptr; }

	return false;
}

void AIComponent::RegisterState(AIState* state) {
	_stateMap.emplace(state->GetName(), state);
}

void AIComponent::AddPoint(const std::string& name, const Vector3D& point) {
	_points[name].emplace_back(point);
}

void AIComponent::InsertPoint(const std::string& name, const Vector3D& point,const int num) {
	_points[name].insert(_points[name].begin() + num, point);
}

void AIComponent::DeletePoint(const std::string& name, int num) {
	//num��0�ȏ�̒l�������ꂽ�ꍇ�́A���̔ԍ��̃R���e�i���폜
	if (num > 0) {
		_points[name].erase(_points[name].begin() + num);
		return;
	}
	//������Ȃ�������A�S�폜
	_points[name].clear();
}

bool AIComponent::IsFound(ObjectBase* target){

	auto targetPos = target->GetPos();

	Vector3D angle = GetOwner()->GetEulerAngle();
	Vector3D pos = GetOwner()->GetPos();

	Vector3D targetToMe(targetPos - pos);

	Vector3D forwardVec(sin(angle.y), 0, cos(angle.y));

	forwardVec = pos + forwardVec * 50.f;

	//�v���C���[�L�����Ƃ̋�����500�ȉ����v���C���[�L�����ʒu�̍���y���������Ȃ�
	if (targetToMe.LengthSquare() <= _viewDist * _viewDist) {

		//����p�͐��ʃx�N�g���Ƃ̍���������p�̔����������王�E�ɓ����Ă���
		if (Vector3D::DotAngle(pos - targetPos, pos - forwardVec, true) < _viewAngle / 2) {

			Segment seg(targetPos, _owner->GetPos() + _view);

			//�I�u�W�F�N�g�ɎՂ�ꂽ��A���삵�Ȃ�
			for (auto&& obj : GetOwner()->GetObjectServer()->GetPhysWorld()->GetFrameComponent()) {

				auto owner = obj->GetOwner();

				MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(
					owner->GetHandle(),
					owner->GetAttachIndex(),
					DxConverter::VecToDx(seg.start),
					DxConverter::VecToDx(seg.end)
				);
				if (hit.HitFlag) { return false; }

			}

			return true;
		}
	}
	return false;
}

bool AIComponent::MoveTo(std::vector<Vector3D>& points, int& num) {
	//points�̃T�C�Y���ړ��������ԍ��ȉ��������ꍇ�A����͏I����Ă���
	//���ł��������������Ă��邪�A������̓G���[���o���Ȃ����߂̏���
	if (points.size() <= num) {
		num = 0;
		return true;
	}

	//���݂̈ʒu���W Y�������𔲂�
	Vector3D notYPos(_owner->GetPos());
	notYPos.y = 0.f;

	//�I�[�i�[�̊p�x
	Vector3D angle = _owner->GetEulerAngle();

	//XZ���������̐��ʃx�N�g��	
	Vector3D forwardVec(sinf(angle.y), 0, cosf(angle.y));
	forwardVec = forwardVec * 50.f;

	//�ړ����������W Y�������𔲂�
	Vector3D arrowPoint(points[num]);
	arrowPoint.y = 0;

	//�������邩
	Vector3D GoalToMe(notYPos - arrowPoint);
	GoalToMe.Normalized();

	float diff = atan2(GoalToMe.x, GoalToMe.z) + DegToRad(180.0f);

	diff -= angle.y;

	//�ړ����x
	float moveSpeed = 2.f;

	//�ړI�n�ɒ�����
	if (Vector3D::LengthSquare(arrowPoint, notYPos) <= 10 * 10) {
		num++;
		//moveSpeed = Vector3D::Length(arrowPoint, notYPos);
	}
	else {
		if (fabs(diff) > 0.0) {
			//���E����
			float crossAngle = Vector3D::CrossAngleXZ(notYPos - (notYPos + forwardVec), notYPos - arrowPoint);

			//��]�X�s�[�h
			float moveAngle = DegToRad(6);

			//crossAngle���̐����ŁA�E��]������]���𔻒肷��
			if (crossAngle > 0.0) {
				if (fabs(diff) < moveAngle) {
					angle.y += diff;
				}
				else {
					angle.y -= moveAngle;
					moveSpeed /= 3;
				}
			}
			else /*if (crossAngle > 0.0) */ {
				if (fabs(diff) < moveAngle) {
					angle.y += diff;
				}
				else {
					angle.y += moveAngle;
					moveSpeed /= 3;
				}
			}
		}
	}
	//�ړ�
	_owner->AddPos(forwardVec.Normalize() * moveSpeed);
	_owner->SetEulerAngle(angle);

	//�ꏄ��������ŏ��̈ʒu��ڎw��
	if (points.size() <= num) {
		num = 0;
		return true;
	}

	return false;
}