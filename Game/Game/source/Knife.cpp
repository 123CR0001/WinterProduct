#include"Knife.h"
#include"CharaBase.h"
#include"ObjectServer.h"
#include"CommonSoldier.h"
#include"FrameComponent.h"
#include"Component.h"
#include"PhysWorld.h"
#include"CapsuleComponent.h"
#include"ModeGame.h"
#include"ModeEffekseer.h"
#include"Player.h"
#include"CameraComponent.h"

bool Knife::Initialize() {

	WeaponBase::Initialize();

	LoadModel("res/Object/sword.mv1");
	_attachIndex = MV1SearchFrame(_handle, "AttackCollision");
	MV1SetupCollInfo(_handle, _attachIndex, 8, 8, 8);

	MV1SetFrameVisible(_handle, _attachIndex, FALSE);

	_scale = Vector3D(1.5f, 2.5f, 1.5f);
	_eulerAngle = Vector3D(0.f,0.f, -PI / 2.f);

	return true;
}

bool Knife::Process() {
	int FrameIndex;
	_matrix = MGetIdent();
	// �t���[��������t���[���ԍ����擾����
	FrameIndex = MV1SearchFrame(_equippedChara->GetHandle(), "Character1_LeftHand");
	_matrix = MGetScale(DxConverter::VecToDx(_scale));
	_matrix = MMult(_matrix, MGetRotX(_eulerAngle.x));
	_matrix = MMult(_matrix, MGetRotZ(_eulerAngle.z));
	_matrix = MMult(_matrix, MV1GetFrameLocalWorldMatrix(_equippedChara->GetHandle(), FrameIndex));
	MV1SetMatrix(_handle, _matrix);

	for (int a = 0; a <= _attachIndex; a++) {
		MV1RefreshCollInfo(_handle, a);
	}
	if (_isAttack) {
		PhysWorld::CollisionDetectionResult result = _frame->GetOverlapResult();
		if (result.isHit && result.item._object != _equippedChara) {
			GetObjectServer()->DeleteObject(result.item._object);
			//�G�t�F�N�g�Đ�
			GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
				"Blood01", 
				result.item.hitPosition,
				Vector3D(0.f, atan2f(result.item.pushVec.x, result.item.pushVec.z), 0.f)
			);
			GetObjectServer()->GetGame()->GetModeEffekseer()->Play(
				"Blood02",
				result.item._object->GetPos()+Vector3D(0.f,10.f,0.f),
				result.item._object->GetEulerAngle()
			);
			//�J�����̗h��
			GetObjectServer()->GetPlayer()->GetCamera()->Swap();
		}
	}

	return true;
}

bool Knife::Render() {

	WeaponBase::Render();

	if (_isAttack) {
		//���b�V�������X�V
		MV1SetupReferenceMesh(_handle, _attachIndex, TRUE);

		//���f���Ɋ܂܂����������Ă���
		MV1_REF_POLYGONLIST list = MV1GetReferenceMesh(_handle, _attachIndex, TRUE);

		//���_�̃C���f�b�N�X
		MV1_REF_POLYGON* poly = list.Polygons;
		//���_�̈ʒu���
		MV1_REF_VERTEX* ver = list.Vertexs;

		//�|���S������\�z
		for (int a = 0; a < list.PolygonNum; a++) {
			DrawTriangle3D(
				ver[poly[a].VIndex[0]].Position,
				ver[poly[a].VIndex[1]].Position,
				ver[poly[a].VIndex[2]].Position,
				GetColor(255, 0, 0),
				FALSE
			);
		}
	}

	return true;
}