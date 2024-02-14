#include"CommonSoldier.h"
#include"Player.h"
#include"ObjectServer.h"
#include"AIComponent.h"
#include"AIPatrol.h"
#include"AIChasePlayer.h"
#include"AIBackPatrol.h"
#include<algorithm>
#include"MoveComponent.h"
#include"AIBlindWalk.h"
#include"CapsuleComponent.h"
#include"FrameComponent.h"
#include"PhysWorld.h"
#include"AICheckPoint.h"
#include"AIBlindWalk.h"

CommonSoldier::CommonSoldier(ObjectServer* server) 
	:CharaBase(server,"CommonSoldier")
	, _AI(NEW AIComponent(this, 1))
	,_detectionLevel(0.f)
	,_visionAngle(0.f)
	,_visionDist(0.f)
	,_moveCom(NEW MoveComponent(this,2))
{

	//AStateの登録
	_AI->RegisterState(NEW AIBackPatrol(_AI));
	_AI->RegisterState(NEW AIChasePlayer(_AI));
	_AI->RegisterState(NEW AIPatrol(_AI));
	_AI->RegisterState(NEW AIBlindWalk(_AI));
	_AI->RegisterState(NEW AICheckPoint(_AI));
	_AI->RegisterState(NEW AIBlindWalk(_AI));

	server->GetCommonSoldiers().emplace_back(this);
	server->GetEnemys().emplace_back(this);
}

CommonSoldier::~CommonSoldier(){
	Terminate();
}

bool CommonSoldier::Initialize() {
	ObjectBase::Initialize();

	LoadModel("res/SDChar/SDChar.mv1");
	// 3Dモデルの1番目のアニメーションをアタッチする
	_attachIndex = 0;
	// アタッチしたアニメーションの総再生時間を取得する

	_isStand = true;

	//視覚
	_visionDist = 500.f;
	_visionAngle = 120.f;


	_capsule->SetMember(180.f, 30.f);

	_scale = Vector3D(1.f, 1.f, 1.f);

	_detectionLevel = 0.f;

	//最初のAI状態
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

	//角度の修正
	if (_eulerAngle.y > 2 * PI) {
		_eulerAngle.y = 0;
	}
	if (_eulerAngle.y <  0) {
		_eulerAngle.y = 2 * PI;
	}

	//検知度の増減(百分率)
	if (IsPlayerFound()) {
		//AIごとに上昇するかしないかを分けるかもしれない
		//ここに書いているのは走り書き
		float per = Vector3D::LengthSquare(GetObjectServer()->GetPlayer()->GetPos(),_pos) / (_visionDist * _visionDist);
		per = -(per - 1);

		//検知度の上昇
		_detectionLevel += per * 0.01;
	}
	else {
		_detectionLevel -= 0.01f;
	}
	_detectionLevel = Clamp(0.f, 1.f, _detectionLevel);

	//重なり修正
	FixPos();

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

	//AIPatrolの巡回経路を登録
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

bool CommonSoldier::IsPlayerFound() {

	auto player = GetObjectServer()->GetPlayer();

	auto playerPos = player->GetPos();

	Vector3D playerToMe(playerPos - _pos);

	Vector3D forwardVec(sin(_eulerAngle.y), 0, cos(_eulerAngle.y));

	forwardVec = _pos + forwardVec * 50.f;

	//プレイヤーキャラとの距離が500以下かつプレイヤーキャラ位置の差にy軸成分がない
	if (playerToMe.LengthSquare() <= _visionDist * _visionDist /*&& fabs(playerToMe.y) < 0.1f*/) {

		//視野角は正面ベクトルとの差分が視野角の半分だったら視界に入っている
		if (Vector3D::DotAngle(_pos - playerPos, _pos - forwardVec, true) < _visionAngle / 2) {

			//_isFound = true;
			//目の高さ
			Vector3D latest(Collision::SegPointLatestPoint(Vector3D(_pos + Vector3D(0, 100, 0)), player->GetCapsuleComponent()->GetCapsule().seg));

			Segment seg(latest, Vector3D(_pos + Vector3D(0, 100, 0)));

			//オブジェクトに遮られたら、動作しない
			for (auto&& obj : GetObjectServer()->GetPhysWorld()->GetFrameComponent()) {

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

bool CommonSoldier::MoveRoute(std::vector<Vector3D>& points, int& num) {

	//pointsのサイズが移動したい番号以下だった場合、巡回は終わっている
	//下でも同じ処理をしているが、こちらはエラーを出さないための処理
	if (points.size() <= num) { 
		num = 0;
		return true;
	}

	//現在の位置座標 Y軸成分を抜く
	Vector3D notYPos(_pos);
	notYPos.y = 0;

	//XZ成分だけの正面ベクトル	
	Vector3D forwardVec(sin(_eulerAngle.y), 0, cos(_eulerAngle.y));
	forwardVec = forwardVec * 50.f;

	//移動したい座標 Y軸成分を抜く
	Vector3D arrowPoint(points[num]);
	arrowPoint.y = 0;

	//差があるか
	Vector3D GoalToMe(notYPos - arrowPoint);
	GoalToMe.Normalized();

	float diff = atan2(GoalToMe.x, GoalToMe.z) + DegToRad(180.0f);

	diff -= _eulerAngle.y;

	//移動速度
	float moveSpeed = 2.f;

	//目的地に着いた
	if (Vector3D::LengthSquare(arrowPoint, notYPos) <= 10 * 10) {
		num ++;
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
					_eulerAngle.y += diff;
				}
				else {
					_eulerAngle.y -= moveAngle;
					moveSpeed /= 3;
				}
			}
			else /*if (crossAngle > 0.0) */{
				if (fabs(diff) < moveAngle) {
					_eulerAngle.y += diff;
				}
				else {
					_eulerAngle.y += moveAngle;
					moveSpeed /= 3;
				}
			}
		}
	}
	//移動
	_pos += forwardVec.Normalize() * moveSpeed;

	//一巡したから最初の位置を目指す
	if (points.size() <= num) {
		num = 0;
		return true;
	}

	return false;
}