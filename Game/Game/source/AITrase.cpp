#include"AITrase.h"
#include"AIComponent.h"
#include"ObjectBase.h"
#include"ObjectServer.h"
#include"Player.h"

AITrase::AITrase(AIComponent* owner)
	:AIState(owner)
	,_pointsNum(0)
{

}

AITrase::~AITrase(){}

void AITrase::OnEnter() {

}

void AITrase::OnExist(){
	_owner->DeletePoint("Trase");
	_pointsNum = 0;
}

bool AITrase::Process() {

	//�ړ�
	_owner->MoveTo(_owner->GetPoints(GetName()), _pointsNum);

	//�o�^���Ă��閼�O�Ɠ������O�����I�u�W�F�N�g�����E�ɓ��ꂽ��
	{
		auto objects = _owner->GetOwner()->GetObjectServer()->GetObjects();
		std::vector<std::string>names;

		names.emplace_back("player");
		names.emplace_back("Decoy");

		for (int a = 0; a < objects.size(); a++) {

			for (int b = 0; b < names.size(); b++) {

				//�o�^�������O��
				if (objects[a]->GetName() != names[b]) { continue; }

				//���E�ɓ����Ă��邩
				if (_owner->IsFound(objects[a])) {
					//�ǂ�������I�u�W�F�N�g�̃A�h���X��o�^
					_owner->SetChaseObject(objects[a]);
					//AIState��ύX
					_owner->ChangeState("Rush");
					return true;

				}
			}
		}
	}

	return true;
}