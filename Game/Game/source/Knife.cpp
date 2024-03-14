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
#include"CharaBase.h"
#include"SoundComponent.h"
#include"ApplicationGlobal.h"

bool Knife::Initialize() {

	WeaponBase::Initialize();

	LoadModel("res/Object/Knife_toDX/Knife.mv1");
	_attachIndex = MV1SearchFrame(_handle, "AttackCollision");
	MV1SetupCollInfo(_handle, _attachIndex, 8, 8, 8);

	MV1SetFrameVisible(_handle, _attachIndex, FALSE);

	_scale = Vector3D(1.f, 0.5f, 1.f);
	_eulerAngle = Vector3D(PI,0.f,0.f);
	return true;
}

bool Knife::Process() {
	int FrameIndex;
	_matrix = MGetIdent();
	// �t���[��������t���[���ԍ����擾����
	FrameIndex = MV1SearchFrame(_equippedChara->GetHandle(), "Owl_RightHandThumb1");
	_matrix = MGetScale(DxConverter::VecToDx(_scale));
	_matrix = MMult(_matrix, MGetRotX(_eulerAngle.x));
	_matrix = MMult(_matrix, MGetRotZ(_eulerAngle.z));
	_matrix = MMult(_matrix, MV1GetFrameLocalWorldMatrix(_equippedChara->GetHandle(), FrameIndex));
	MV1SetMatrix(_handle, _matrix);

	MV1RefreshCollInfo(_handle, _attachIndex);

	if (_isAttack) {
		PhysWorld::CollisionDetectionResult result = _frame->GetOverlapResult();

		//�J�v�Z���R���|�[�l���g�ƂԂ��������A�����𑕔����Ă���L��������Ȃ���
		if (result.isHit 
			&& result.item._object != _equippedChara 
			&& result.item._object->GetState() == STATE::kActive
			) {

			CharaBase::DamageData damageData = { true,this,result.item };

			result.item._object->ApplyDamage(damageData);

			//���𐶐�
			NEW SoundComponent(result.item._object, 300.f);

			gGlobal._sndServer.Get("SE_03")->Play();
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