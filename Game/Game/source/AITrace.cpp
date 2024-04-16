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
	//“o˜^‚µ‚½À•W‚ðíœ
	_owner->DeletePoint("Trace");
	//À•W‚Ì”‚ð‰Šú‰»
	_pointsNum = 0;
}

bool AITrace::Process() {

	//ˆÚ“®
	_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);

	//“o˜^‚µ‚Ä‚¢‚é–¼‘O‚Æ“¯‚¶–¼‘O‚ðŽ‚ÂƒIƒuƒWƒFƒNƒg‚ðŽ‹ŠE‚É“ü‚ê‚½‚©
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			for (int b = 0; b < names.size(); b++) {

				//“o˜^‚µ‚½–¼‘O‚©
				if (objects[a]->GetName() != names[b]) { continue; }

				//Ž‹ŠE‚É“ü‚Á‚Ä‚¢‚é‚©
				if (_owner->IsFound(objects[a])) {
					//’Ç‚¢‚©‚¯‚éƒIƒuƒWƒFƒNƒg‚ÌƒAƒhƒŒƒX‚ð“o˜^
					_owner->SetChaseObject(objects[a]);
					//AIState‚ð•ÏX
					_owner->ChangeState("Rush");
					return true;

				}
			}
		}
	}

	return true;
}