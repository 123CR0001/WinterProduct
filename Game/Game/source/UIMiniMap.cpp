#include"UIMiniMap.h"
#include"ModeGame.h"
#include"ObjectServer.h"
#include"PhysWorld.h"
#include"FrameComponent.h"
#include"ApplicationMain.h"
#include"ObjectBase.h"
#include"Player.h"
#include"CameraComponent.h"

UIMiniMap::UIMiniMap(ModeGame* game)
	:_game(game)
	, _maxPos(Vector3D(-9999999.f, 0.f, -9999999.f))
	, _minPos(Vector3D(9999999.f, 0.f, 9999999.f))
{
	_mapTextHandle = ResourceServer::LoadGraph("res/MiniMap/mapstage1.png");

	{
		auto& frames = _game->GetObjectServer()->GetPhysWorld()->GetFrameComponent();

		//�S�ẴI�u�W�F�N�g�̃��f������ő�E�ŏ����_���Z�o
		for(int a = 0; a < frames.size(); a++) {
			//���f���̃n���h�����擾
			const int handle = frames[a]->GetOwner()->GetHandle();

			//���b�V�����Z�b�g�A�b�v���X�V
			MV1SetupReferenceMesh(handle, -1, TRUE);
			MV1RefreshReferenceMesh(handle, -1, TRUE);

			//���f���̍ő�E�ŏ����_���擾
			auto result = MV1GetReferenceMesh(handle, -1, TRUE);

			//xz�̊e�l����ԑ傫�������ő咸�_�̐����ɂ���
			if(_maxPos.x < result.MaxPosition.x) { _maxPos.x = result.MaxPosition.x; }
			if(_maxPos.z < result.MaxPosition.z) { _maxPos.z = result.MaxPosition.y; }

			//xz�̊e�l����ԏ����������ŏ����_�̐����ɂ���
			if(_minPos.x > result.MinPosition.x) { _minPos.x = result.MinPosition.x; }
			if(_minPos.z > result.MinPosition.z) { _minPos.z = result.MinPosition.y; }
		}
		//�ő咸�_����ŏ����_�̂��傤�ǔ����̈ʒu
		_middlePos = Vector3D::LineInter(_maxPos, _minPos, 0.5f);
	}

	//�摜�̕`��ʒu
	_x = ApplicationMain::GetInstance()->GetInstance()->DispSizeW() - 225;
	_y = 160;

	_mag = 0.2f;

	//�摜�̕`��T�C�Y���g��k�����ɍ��킹��
	_w = static_cast<int>(fabsf(_maxPos.x - _minPos.x) * _mag);
	_h = static_cast<int>(fabsf(_maxPos.z - _minPos.z) * _mag);


	_mapScreen = MakeScreen(_w, _h, TRUE);
	_clipScreen = MakeScreen(_w, _h, FALSE);
	_mixScreen = MakeScreen(_w, _h, TRUE);

	//�`�悷��~�j�}�b�v�̉~�`��̔��a
	_radius = 100.f;

}

UIMiniMap::~UIMiniMap() {
	DeleteGraph(_clipScreen);
	DeleteGraph(_mapScreen);
	DeleteGraph(_mixScreen);
}

bool UIMiniMap::Process() {
	//�v���C���[�̃A�h���X���擾
	auto player = _game->GetObjectServer()->GetPlayer();

	//�v����[�̃A�h���X���Ȃ���΁A���������Ȃ�
	if(!player) { return false; }

	//�v���C���[�̈ʒu�����擾
	auto playerPos = player->GetPos();

	//_middlePos(�}�b�v�̒��S��)����v���C���[�ւ̃x�N�g������`�⊮�ŁA_mag�{�����x�N�g���@= _middlePos���猩�āA���������̂܂܂ɁA�߂Â����v���C���[�̈ʒu(���[���h���W)
	//��������A_middlePos�������ƁA�}�b�v�̒��S����v���C���[�ւ̃x�N�g��(���[�J�����W)
	auto vecMapPlayerMiddle = Vector3D::LineInter(_middlePos, playerPos, _mag) - _middlePos;

	vecMapPlayerMiddle.z *= -1.f;

	//�~�j�}�b�v�͌��_(0,0)�ɕ`�悷��̂ŁA�T�C�Y/2���~�j�}�b�v�̒��S���W�ɂȂ�
	_mapPlayerPos = Vector3D((float)_w / 2, 0.f, (float)_h / 2) + vecMapPlayerMiddle;

	// �ꎞ�摜��`��Ώۂɐݒ肵�ăN���A
	SetDrawScreen(_mapScreen);
	ClearDrawScreen();

	//�x�[�X���n
	DrawBox(-10, -10, _w + 10, _h + 10, GetColor(0, 10, 0), TRUE);
	//�~�j�}�b�v��`��
	DrawExtendGraph(0, 0, _w, _h, _mapTextHandle, TRUE);


	//�~�j�}�b�v�̏�ɃL�����̈ʒu��`��
	DrawCircleAA(_mapPlayerPos.x, _mapPlayerPos.z, 3.f, 40, GetColor(255, 0, 0), TRUE);

	SetFontSize(12);
	for(auto&& obj : _game->GetObjectServer()->GetObjects()) {
		if(obj == player) { continue; }

		bool is = false;

		for(auto&& frame : _game->GetObjectServer()->GetPhysWorld()->GetFrameComponent()) {
			if(frame->GetOwner() == obj) {
				is = true;
				break;
			}
		}

		if(is) { continue; }

		auto pos = obj->GetPos();

		auto vecMapObjMiddle = Vector3D::LineInter(_middlePos, pos, _mag) - _middlePos;

		vecMapObjMiddle.z *= -1.f;

		auto mapPos = Vector3D((float)_w / 2, 0.f, (float)_h / 2) + vecMapObjMiddle;

		DrawCircleAA(mapPos.x, mapPos.z, 3.f, 40, GetColor(255, 0, 0), TRUE);

		std::string name = obj->GetName();
		int size = name.size();
		int centerX = 0;
		int centerY = 0;

		DrawRotaFormatString(mapPos.x - centerX, mapPos.z - centerY,
			1.0, 1.0,
			centerX, centerY,
			player->GetCamera()->GetAngle().y + PI,
			GetColor(255, 255, 255), GetColor(255, 255, 255),
			FALSE,
			"%s", name.c_str()
		);
	}

	/*�n�}�ƍ���*/
	SetDrawScreen(_clipScreen);
	ClearDrawScreen();

	//�~�j�}�b�v��̃v���C���[�̈ʒu�𒆐S�ɁA�~��`��
	DrawCircleAA(_mapPlayerPos.x, _mapPlayerPos.z, _radius, 40, GetColor(255, 255, 255), TRUE);

	GraphBlendBlt(_mapScreen, _clipScreen, _mixScreen, 255,
		DX_GRAPH_BLEND_RGBA_SELECT_MIX,
		DX_RGBA_SELECT_SRC_R,	// ������� R �� GrHandle �� R
		DX_RGBA_SELECT_SRC_G,	// ������� G �� GrHandle �� G
		DX_RGBA_SELECT_SRC_B,	// ������� B �� GrHandle �� B
		DX_RGBA_SELECT_BLEND_R	// ������� A �� TempScreen �� R
	);


	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ̃N���A
	ClearDrawScreen();

	return true;
}

bool UIMiniMap::Draw() {
	//// ��ʂɍ��������摜��`�悷��
	const float diffX = (float)_x - _mapPlayerPos.x * 2;
	const float diffY = (float)_y - _mapPlayerPos.z * 2;

	const float angle = _game->GetObjectServer()->GetPlayer()->GetCamera()->GetAngle().y;

	VECTOR pos[4] = {
		//����
		VGet(0 - _mapPlayerPos.x,	0 - _mapPlayerPos.z,0),
		//�E��
		VGet(_w - _mapPlayerPos.x,	0 - _mapPlayerPos.z,0),
		//�E��
		VGet(_w - _mapPlayerPos.x,	_h - _mapPlayerPos.z,0),
		//����
		VGet(0 - _mapPlayerPos.x,	_h - _mapPlayerPos.z,0)
	};

	MATRIX m = MGetRotZ(angle);
	m = MMult(m, MGetTranslate(VGet(_x, _y, 0.f)));

	for(int a = 0; a < 4; a++) {
		pos[a] = VTransform(pos[a], m);
	}


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCircleAA(_x, _y, _radius + 3.f, 40, GetColor(255, 255, 0), TRUE);
	DrawCircleAA(_x, _y, _radius, 40, GetColor(0, 0, 0), TRUE);
	DrawModiGraphF(
		pos[0].x, pos[0].y,
		pos[1].x, pos[1].y,
		pos[2].x, pos[2].y,
		pos[3].x, pos[3].y,
		_mixScreen,
		TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return true;
}