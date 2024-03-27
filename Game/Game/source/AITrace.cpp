#include"AITrace.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"Player.h"

AITrace::AITrace(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AITrace::~AITrace(){}

void AITrace::OnEnter() {

}

void AITrace::OnExist(){
	_owner->DeletePoint("Trace");
	_pointsNum = 0;
}

bool AITrace::Process() {

	//移動
	_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);

	//登録している名前と同じ名前を持つオブジェクトを視界に入れたか
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			for (int b = 0; b < names.size(); b++) {

				//登録した名前か
				if (objects[a]->GetName() != names[b]) { continue; }

				//視界に入っているか
				if (_owner->IsFound(objects[a])) {
					//追いかけるオブジェクトのアドレスを登録
					_owner->SetChaseObject(objects[a]);
					//AIStateを変更
					_owner->ChangeState("Rush");
					return true;

				}
			}
		}
	}

	return true;
}