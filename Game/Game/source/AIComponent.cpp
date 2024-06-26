#include"AIComponent.h"
#include"AIState.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"PhysWorld.h"
#include"FrameComponent.h"

#include"ModeGame.h"

#include<algorithm>

AIComponent::AIComponent(ObjectBase* owner,int order)
	:Component(owner,order)
	,_currentState(nullptr)
	,_chaseObj(nullptr)
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

void AIComponent::AddPoint(const std::string& name, const Vector3& point) {
	_points[name].emplace_back(point);
}

void AIComponent::InsertPoint(const std::string& name, const Vector3& point,const int num) {
	_points[name].insert(_points[name].begin() + num, point);
}

void AIComponent::DeletePoint(const std::string& name, int num) {
	//numに0以上の値が入れられた場合は、その番号のコンテナを削除
	if (num > 0) {
		_points[name].erase(_points[name].begin() + num);
		return;
	}
	//入れられなかったら、全削除
	_points[name].clear();
}

bool AIComponent::IsFound(ObjectBase* target){

	auto targetPos = target->GetPos();

	Vector3 angle = GetOwner()->GetEulerAngle();
	Vector3 pos = GetOwner()->GetPos();

	Vector3 targetToMe(targetPos - pos);

	Vector3 forwardVec(sin(angle.y), 0, cos(angle.y));

	forwardVec = pos + forwardVec * 50.f;

	//プレイヤーキャラとの距離が500以下かつプレイヤーキャラ位置の差にy軸成分がない
	if (targetToMe.LengthSquare() <= _viewDist * _viewDist) {

		//視野角は正面ベクトルとの差分が視野角の半分だったら視界に入っている
		if (Vector3::DotAngle(pos - targetPos, pos - forwardVec, true) < _viewAngle / 2) {

			Segment seg(targetPos, _owner->GetPos() + _viewLoacalPos);

			//オブジェクトに遮られたら、見えていない
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

ObjectBase* AIComponent::IsFound() {

	for (int a = 0; a < (int)_owner->GetObjectServer()->GetObjects().size(); ++a) {

		auto object = _owner->GetObjectServer()->GetObjects()[a];

		for (int b = 0; b < (int)_chaseObjectName.size(); ++b) {
			
			if (object->GetName() != _chaseObjectName[b]) { continue; }

			if (IsFound(object)) {
				return object;
			}
		}
	}

	return nullptr;
}

bool AIComponent::MoveTo(std::vector<Vector3>& points, int& num) {
	//pointsのサイズが移動したい番号以下だった場合、巡回は終わっている
	//下でも同じ処理をしているが、こちらはエラーを出さないための処理
	if (points.size() <= num) {
		return false;
	}

	//現在の位置座標 Y軸成分を抜く
	Vector3 notYPos(_owner->GetPos());
	notYPos.y = 0.f;

	//オーナーの角度
	Vector3 angle = _owner->GetEulerAngle();

	//XZ成分だけの正面ベクトル	
	Vector3 forwardVec(sinf(angle.y), 0, cosf(angle.y));
	forwardVec = forwardVec * 50.f;

	//移動したい座標 Y軸成分を抜く
	Vector3 arrowPoint(points[num]);
	arrowPoint.y = 0;

	//差があるか
	Vector3 GoalToMe(notYPos - arrowPoint);
	GoalToMe.Normalized();

	float diff = atan2(GoalToMe.x, GoalToMe.z) + DegToRad(180.0f);

	diff -= angle.y;

	//移動速度
	float moveSpeed = 2.f;

	//目的地に着いた
	if (Vector3::Equal(arrowPoint,notYPos,10.f)) {
		num++;
		//moveSpeed = Vector3D::Length(arrowPoint, notYPos);
	}
	else {
		if (fabs(diff) > 0.0) {
			//左右判定
			float crossAngle = Vector3::CrossAngleXZ(notYPos - (notYPos + forwardVec), notYPos - arrowPoint);

			//回転スピード
			float moveAngle = DegToRad(6);

			//crossAngleがの正負で、右回転か左回転化を判定する
			//fabs(diff) < moveAngleの判定は、回転角度がmoveAngleより小さかったら、その角度に合わせる
			if (crossAngle > 0.0f) {
				if (fabs(diff) < moveAngle) {
					angle.y += diff;
				}
				else {
					angle.y -= moveAngle;
					moveSpeed /= 3;
				}
			}
			else {
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
	//移動
	_owner->AddPos(forwardVec.Normalize() * moveSpeed);
	_owner->SetEulerAngle(angle);

	//一巡した
	if (points.size() <= num) {
		num = 0;
		return true;
	}

	return false;
}