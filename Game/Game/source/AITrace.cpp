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
	//�o�^�������W���폜
	_owner->DeletePoint("Trace");
	//���W�̐���������
	_pointsNum = 0;
}

bool AITrace::Process() {

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