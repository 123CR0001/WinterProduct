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

	Vector3D angle = GetOwner()->GetEulerAngle();
	Vector3D pos = GetOwner()->GetPos();

	Vector3D targetToMe(targetPos - pos);

	Vector3D forwardVec(sin(angle.y), 0, cos(angle.y));

	forwardVec = pos + forwardVec * 50.f;

	//プレイヤーキャラとの距離が500以下かつプレイヤーキャラ位置の差にy軸成分がない
	if (targetToMe.LengthSquare() <= _viewDist * _viewDist) {

		//視野角は正面ベクトルとの差分が視野角の半分だったら視界に入っている
		if (Vector3D::DotAngle(pos - targetPos, pos - forwardVec, true) < _viewAngle / 2) {

			Segment seg(targetPos, _owner->GetPos() + _view);

			//オブジェクトに遮られたら、動作しない
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
	//pointsのサイズが移動したい番号以下だった場合、巡回は終わっている
	//下でも同じ処理をしているが、こちらはエラーを出さないための処理
	if (points.size() <= num) {
		num = 0;
		return true;
	}

	//現在の位置座標 Y軸成分を抜く
	Vector3D notYPos(_owner->GetPos());
	notYPos.y = 0.f;

	//オーナーの角度
	Vector3D angle = _owner->GetEulerAngle();

	//XZ成分だけの正面ベクトル	
	Vector3D forwardVec(sinf(angle.y), 0, cosf(angle.y));
	forwardVec = forwardVec * 50.f;

	//移動したい座標 Y軸成分を抜く
	Vector3D arrowPoint(points[num]);
	arrowPoint.y = 0;

	//差があるか
	Vector3D GoalToMe(notYPos - arrowPoint);
	GoalToMe.Normalized();

	float diff = atan2(GoalToMe.x, GoalToMe.z) + DegToRad(180.0f);

	diff -= angle.y;

	//移動速度
	float moveSpeed = 2.f;

	//目的地に着いた
	if (Vector3D::LengthSquare(arrowPoint, notYPos) <= 10 * 10) {
		num++;
		//moveSpeed = Vector3D::Length(arrowPoint, notYPos);
	}
	else {
		if (fabs(diff) > 0.0) {
			//左右判定
			float crossAngle = Vector3D::CrossAngleXZ(notYPos - (notYPos + forwardVec), notYPos - arrowPoint);

			//回転スピード
			float moveAngle = DegToRad(6);

			//crossAngleがの正負で、右回転か左回転化を判定する
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
	//移動
	_owner->AddPos(forwardVec.Normalize() * moveSpeed);
	_owner->SetEulerAngle(angle);

	//一巡したから最初の位置を目指す
	if (points.size() <= num) {
		num = 0;
		return true;
	}

	return false;
}