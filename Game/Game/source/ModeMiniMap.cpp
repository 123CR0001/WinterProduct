#include"ModeMiniMap.h"
#include"ModeGame.h"

#include"ObjectServer.h"
#include"PhysWorld.h"

#include"FrameComponent.h"
#include"CameraComponent.h"

#include"ObjectBase.h"
#include"Player.h"

#include"ApplicationMain.h"

ModeMiniMap::ModeMiniMap(ModeGame* game)
	:_game(game)
	, _maxPos(Vector3D(-9999999.f, 0.f, -9999999.f))
	, _minPos(Vector3D(9999999.f, 0.f, 9999999.f))
{
	_mapTextHandle = LoadGraph("res/MiniMap/mapstage1.png");
}

ModeMiniMap::~ModeMiniMap() {
	DeleteGraph(_clipScreen);
	DeleteGraph(_mapScreen);
	DeleteGraph(_mixScreen);
}

bool ModeMiniMap::Initialize() {
	{
		auto& frames = _game->GetObjectServer()->GetPhysWorld()->GetFrameComponent();

		//全てのオブジェクトのモデルから最大・最小頂点を算出
		for(int a = 0; a < frames.size(); a++) {
			//モデルのハンドルを取得
			const int handle = frames[a]->GetOwner()->GetHandle();

			//メッシュをセットアップ＆更新
			MV1SetupReferenceMesh(handle, -1, TRUE);
			MV1RefreshReferenceMesh(handle, -1, TRUE);

			//モデルの最大・最小頂点を取得
			auto result = MV1GetReferenceMesh(handle, -1, TRUE);

			//xzの各値が一番大きい物を最大頂点の成分にする
			if(_maxPos.x < result.MaxPosition.x) { _maxPos.x = result.MaxPosition.x; }
			if(_maxPos.z < result.MaxPosition.z) { _maxPos.z = result.MaxPosition.y; }

			//xzの各値が一番小さい物を最小頂点の成分にする
			if(_minPos.x > result.MinPosition.x) { _minPos.x = result.MinPosition.x; }
			if(_minPos.z > result.MinPosition.z) { _minPos.z = result.MinPosition.y; }
		}
		//最大頂点から最小頂点のちょうど半分の位置
		_middlePos = Vector3D::LineInter(_maxPos, _minPos, 0.5f);
	}

	//画像の描画位置
	_x = ApplicationMain::GetInstance()->GetInstance()->DispSizeW() - 150;
	_y = 100;

	_mag = 0.1f;

	//画像の描画サイズを拡大縮小率に合わせる
	_w = static_cast<int>(fabsf(_maxPos.x - _minPos.x) * _mag);
	_h = static_cast<int>(fabsf(_maxPos.z - _minPos.z) * _mag);


	_mapScreen =	MakeScreen(_w, _h, TRUE);
	_clipScreen =	MakeScreen(_w, _h, FALSE);
	_mixScreen =	MakeScreen(_w, _h, TRUE);

	return true;
}

bool ModeMiniMap::Process() {

	//プレイヤーのアドレスを取得
	auto player = _game->GetObjectServer()->GetPlayer();

	//プレやーのアドレスがなければ、処理をしない
	if(!player) { return false; }

	//プレイヤーの位置情報を取得
	auto playerPos = player->GetPos();

	//_middlePos(マップの中心手)からプレイヤーへのベクトルを線形補完で、_mag倍したベクトル　= _middlePosから見て、方向をそのままに、近づけたプレイヤーの位置(ワールド座標)
	//そこから、_middlePosを引くと、マップの中心からプレイヤーへのベクトル(ローカル座標)
	auto vecMapPlayerMiddle = Vector3D::LineInter(_middlePos, playerPos, _mag) - _middlePos;

	vecMapPlayerMiddle.z *= -1.f;

	//ミニマップは原点(0,0)に描画するので、サイズ/2がミニマップの中心座標になる
	_mapPlayerPos = Vector3D((float)_w / 2, 0.f, (float)_h / 2) + vecMapPlayerMiddle;

	// 一時画像を描画対象に設定してクリア
	SetDrawScreen(_mapScreen);
	ClearDrawScreen();

	//ミニマップを描画
	DrawExtendGraph(0, 0, _w, _h, _mapTextHandle, TRUE);

	//ミニマップの上にキャラの位置を描画
	DrawCircleAA(_mapPlayerPos.x, _mapPlayerPos.z, 3.f, 40, GetColor(255, 0, 0), TRUE);

	/*地図と合成*/
	SetDrawScreen(_clipScreen);
	ClearDrawScreen();

	//ミニマップ上のプレイヤーの位置を中心に、円を描画
	DrawCircleAA(_mapPlayerPos.x, _mapPlayerPos.z, 170.f, 40, GetColor(255, 255, 255), TRUE);

	GraphBlendBlt(_mapScreen, _clipScreen, _mixScreen, 255,
		DX_GRAPH_BLEND_RGBA_SELECT_MIX,
		DX_RGBA_SELECT_SRC_R,	// 合成後の R は GrHandle の R
		DX_RGBA_SELECT_SRC_G,	// 合成後の G は GrHandle の G
		DX_RGBA_SELECT_SRC_B,	// 合成後の B は GrHandle の B
		DX_RGBA_SELECT_BLEND_R	// 合成後の A は TempScreen の R
	);


	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面のクリア
	ClearDrawScreen();
	return true;
}

bool ModeMiniMap::Render() {

	//// 画面に合成した画像を描画する
	const float diffX = (float)_x - _mapPlayerPos.x * 2;
	const float diffY = (float)_y - _mapPlayerPos.z * 2;

	const float angle = _game->GetObjectServer()->GetPlayer()->GetCamera()->GetAngle().y;

	VECTOR pos[4] = {
		//左上
		VGet(0 - _mapPlayerPos.x,	0 - _mapPlayerPos.z,0),
		//右上
		VGet(_w - _mapPlayerPos.x,	0 - _mapPlayerPos.z,0),
		//右下
		VGet(_w - _mapPlayerPos.x,	_h - _mapPlayerPos.z,0),
		//左下
		VGet(0 - _mapPlayerPos.x,	_h - _mapPlayerPos.z,0)
	};

	MATRIX m = MGetRotZ(angle);
	m = MMult(m, MGetTranslate(VGet(_x, _y, 0.f)));

	for(int a = 0; a < 4; a++) {
		pos[a] = VTransform(pos[a], m);
	}


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCircleAA(_x, _y, 173.f, 40, GetColor(255, 255, 0), TRUE);
	DrawCircleAA(_x, _y, 170.f, 40, GetColor(0, 0, 0), TRUE);
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