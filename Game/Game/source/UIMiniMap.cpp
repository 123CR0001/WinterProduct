#include"UIMiniMap.h"
#include"ModeGame.h"
#include"ObjectServer.h"
#include"PhysWorld.h"
#include"FrameComponent.h"
#include"ApplicationMain.h"
#include"ObjectBase.h"
#include"Player.h"
#include"CameraComponent.h"

UIMiniMap::UIMiniMap(ModeGame* game, int drawOrder)
	:UI(drawOrder)
	,_game(game)
	, _maxPos(Vector3(-9999999.f, 0.f, -9999999.f))
	, _minPos(Vector3(9999999.f, 0.f, 9999999.f))
	,_zoom(1.f)
	,_x(0.f)
	,_y(0.f)
	,_mag(1.f)
	,_mapScreen(0)
	,_mixScreen(0)
	,_clipScreen(0)
{

	std::string stage = _game->GetStage().substr(0, 1);
	std::string area = _game->GetStage().substr(2, 1);
	std::string str = "res/stage/stage" + stage + "/" + area + "/mapstage.png";
	_mapTextHandle = ResourceServer::LoadGraph(str.c_str());
	{
		auto& frames = _game->GetObjectServer()->GetPhysWorld()->GetFrameComponent();

		//�}�b�v���f�����Ȃ�
		if(frames.size() == 0)return;

		//�S�ẴI�u�W�F�N�g�̃��f������ő�E�ŏ����_���Z�o
		for(int a = 0; a < frames.size(); a++) {
			frames[a]->GetOwner()->ModelMatrixSetUp();

			//���f���̃n���h�����擾
			const int handle = frames[a]->GetOwner()->GetHandle();

			//���b�V�����Z�b�g�A�b�v���X�V
			MV1SetupReferenceMesh(handle, -1, TRUE);
			MV1RefreshReferenceMesh(handle, -1, TRUE);

			//���f���̍ő�E�ŏ����_���擾
			auto result = MV1GetReferenceMesh(handle, -1, TRUE);

			//xz�̊e�l����ԑ傫�������ő咸�_�̐����ɂ���
			if(_maxPos.x < result.MaxPosition.x) { _maxPos.x = result.MaxPosition.x; }
			if(_maxPos.z < result.MaxPosition.z) { _maxPos.z = result.MaxPosition.z; }

			//xz�̊e�l����ԏ����������ŏ����_�̐����ɂ���
			if(_minPos.x > result.MinPosition.x) { _minPos.x = result.MinPosition.x; }
			if(_minPos.z > result.MinPosition.z) { _minPos.z = result.MinPosition.z; }
		}
		//�ő咸�_����ŏ����_�̂��傤�ǔ����̈ʒu
		_centerPos = Vector3::Lerp(_maxPos, _minPos, 0.5f);
	}

	//�摜�̕`��ʒu
	_x = 1520.f * SCREEN_WIDTH_MAG;
	_y = 200.f * SCREEN_HEIGHT_MAG;

	_mag = 0.2f;

	//�摜�̕`��T�C�Y���g��k�����ɍ��킹��
	_w = static_cast<int>(fabsf(_maxPos.x - _minPos.x) * _mag);
	_h = static_cast<int>(fabsf(_maxPos.z - _minPos.z) * _mag);

	// �����ǂݍ��ݐݒ�
	_mapScreen = MakeScreen(_w, _h, TRUE);
	_clipScreen = MakeScreen(_w, _h, FALSE);
	_mixScreen = MakeScreen(_w, _h, TRUE);

	//�`�悷��~�j�}�b�v�̉~�`��̔��a
	_radius = 150.f * SCREEN_WIDTH_MAG;


	_identiColor["player"] = GetColor(0, 0, 255);
	_identiColor["CommonSoldier"] = GetColor(255, 0, 0);
	_identiColor["Energy"] = GetColor(255, 0, 255);
	_identiColor["Siren"] = GetColor(255, 255, 0);


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

	//�~�j�}�b�v�̒��S�n����v���C���[�̈ʒu�܂ł̃x�N�g��
	auto vecMapPlayerCenter = (playerPos - _centerPos) * _mag;

	//z�����𔽓]
	vecMapPlayerCenter.z *= -1.f;

	//�~�j�}�b�v�͌��_(0,0)�ɕ`�悷��̂ŁA�T�C�Y/2���~�j�}�b�v�̒��S���W�ɂȂ�
	_mapPlayerPos = Vector3((float)_w / 2.f, 0.f, (float)_h / 2.f) + vecMapPlayerCenter;

	// �ꎞ�摜��`��Ώۂɐݒ肵�ăN���A
	SetDrawScreen(_mapScreen);
	ClearDrawScreen();

	//�x�[�X���n
	DrawBox(0 , 0 ,  _w , _h, GetColor(125, 125, 125), TRUE);

	//�~�j�}�b�v��`��
	DrawExtendGraph(0, 0, _w, _h , _mapTextHandle, TRUE);


	for(auto&& obj : _game->GetObjectServer()->GetObjects()) {
		
		//�o�^����Ă��Ȃ�������A�`�悵�Ȃ�
		if(_identiColor.find(obj->GetName()) == _identiColor.end()) { continue; }

		auto pos = obj->GetPos();

		//���S�n�_���猩���I�u�W�F�N�g�̃x�N�g��
		auto vecMapObjMiddle = (pos - _centerPos) * _mag;

		//z�����𔽓]
		vecMapObjMiddle.z *= -1.f;

		auto mapPos = Vector3((float)_w / 2.f, 0.f, (float)_h / 2.f) + vecMapObjMiddle;

		DrawCircleAA(mapPos.x , mapPos.z , 3.f, 32, _identiColor[obj->GetName()], TRUE);

	}

	//�n�}�ƍ���
	SetDrawScreen(_clipScreen);
	ClearDrawScreen();

	//�~�j�}�b�v��̃v���C���[�̈ʒu�𒆐S�ɁA�~��`��
	DrawCircleAA(_mapPlayerPos.x , _mapPlayerPos.z , _radius, 40, GetColor(255, 255, 255), TRUE);

	GraphBlendBlt(_mapScreen, _clipScreen, _mixScreen, 255,
		DX_GRAPH_BLEND_RGBA_SELECT_MIX,
		DX_RGBA_SELECT_SRC_R,	// ������� R �� GrHandle �� R
		DX_RGBA_SELECT_SRC_G,	// ������� G �� GrHandle �� G
		DX_RGBA_SELECT_SRC_B,	// ������� B �� GrHandle �� B
		DX_RGBA_SELECT_BLEND_R	// ������� A �� TempScreen �� R
	);


	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

bool UIMiniMap::Draw() {
	//��ʂɍ��������摜��`�悷��

	const float angle = _game->GetObjectServer()->GetPlayer()->GetCamera()->GetAngle().y;

	//�v���C���[�����S�ɗ���悤�ɁA�摜��`��
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