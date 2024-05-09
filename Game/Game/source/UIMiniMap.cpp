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

		//マップモデルがない
		if(frames.size() == 0)return;

		//全てのオブジェクトのモデルから最大・最小頂点を算出
		for(int a = 0; a < frames.size(); a++) {
			frames[a]->GetOwner()->ModelMatrixSetUp();

			//モデルのハンドルを取得
			const int handle = frames[a]->GetOwner()->GetHandle();

			//メッシュをセットアップ＆更新
			MV1SetupReferenceMesh(handle, -1, TRUE);
			MV1RefreshReferenceMesh(handle, -1, TRUE);

			//モデルの最大・最小頂点を取得
			auto result = MV1GetReferenceMesh(handle, -1, TRUE);

			//xzの各値が一番大きい物を最大頂点の成分にする
			if(_maxPos.x < result.MaxPosition.x) { _maxPos.x = result.MaxPosition.x; }
			if(_maxPos.z < result.MaxPosition.z) { _maxPos.z = result.MaxPosition.z; }

			//xzの各値が一番小さい物を最小頂点の成分にする
			if(_minPos.x > result.MinPosition.x) { _minPos.x = result.MinPosition.x; }
			if(_minPos.z > result.MinPosition.z) { _minPos.z = result.MinPosition.z; }
		}
		//最大頂点から最小頂点のちょうど半分の位置
		_centerPos = Vector3::Lerp(_maxPos, _minPos, 0.5f);
	}

	//画像の描画位置
	_x = 1520.f * SCREEN_WIDTH_MAG;
	_y = 200.f * SCREEN_HEIGHT_MAG;

	_mag = 0.2f;

	//画像の描画サイズを拡大縮小率に合わせる
	_w = static_cast<int>(fabsf(_maxPos.x - _minPos.x) * _mag);
	_h = static_cast<int>(fabsf(_maxPos.z - _minPos.z) * _mag);

	// 同期読み込み設定
	_mapScreen = MakeScreen(_w, _h, TRUE);
	_clipScreen = MakeScreen(_w, _h, FALSE);
	_mixScreen = MakeScreen(_w, _h, TRUE);

	//描画するミニマップの円形上の半径
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

	//プレイヤーのアドレスを取得
	auto player = _game->GetObjectServer()->GetPlayer();

	//プレやーのアドレスがなければ、処理をしない
	if(!player) { return false; }

	//プレイヤーの位置情報を取得
	auto playerPos = player->GetPos();

	//ミニマップの中心地からプレイヤーの位置までのベクトル
	auto vecMapPlayerCenter = (playerPos - _centerPos) * _mag;

	//z成分を反転
	vecMapPlayerCenter.z *= -1.f;

	//ミニマップは原点(0,0)に描画するので、サイズ/2がミニマップの中心座標になる
	_mapPlayerPos = Vector3((float)_w / 2.f, 0.f, (float)_h / 2.f) + vecMapPlayerCenter;

	// 一時画像を描画対象に設定してクリア
	SetDrawScreen(_mapScreen);
	ClearDrawScreen();

	//ベース生地
	DrawBox(0 , 0 ,  _w , _h, GetColor(125, 125, 125), TRUE);

	//ミニマップを描画
	DrawExtendGraph(0, 0, _w, _h , _mapTextHandle, TRUE);


	for(auto&& obj : _game->GetObjectServer()->GetObjects()) {
		
		//登録されていなかったら、描画しない
		if(_identiColor.find(obj->GetName()) == _identiColor.end()) { continue; }

		auto pos = obj->GetPos();

		//中心地点から見たオブジェクトのベクトル
		auto vecMapObjMiddle = (pos - _centerPos) * _mag;

		//z成分を反転
		vecMapObjMiddle.z *= -1.f;

		auto mapPos = Vector3((float)_w / 2.f, 0.f, (float)_h / 2.f) + vecMapObjMiddle;

		DrawCircleAA(mapPos.x , mapPos.z , 3.f, 32, _identiColor[obj->GetName()], TRUE);

	}

	//地図と合成
	SetDrawScreen(_clipScreen);
	ClearDrawScreen();

	//ミニマップ上のプレイヤーの位置を中心に、円を描画
	DrawCircleAA(_mapPlayerPos.x , _mapPlayerPos.z , _radius, 40, GetColor(255, 255, 255), TRUE);

	GraphBlendBlt(_mapScreen, _clipScreen, _mixScreen, 255,
		DX_GRAPH_BLEND_RGBA_SELECT_MIX,
		DX_RGBA_SELECT_SRC_R,	// 合成後の R は GrHandle の R
		DX_RGBA_SELECT_SRC_G,	// 合成後の G は GrHandle の G
		DX_RGBA_SELECT_SRC_B,	// 合成後の B は GrHandle の B
		DX_RGBA_SELECT_BLEND_R	// 合成後の A は TempScreen の R
	);


	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

bool UIMiniMap::Draw() {
	//画面に合成した画像を描画する

	const float angle = _game->GetObjectServer()->GetPlayer()->GetCamera()->GetAngle().y;

	//プレイヤーが中心に来るように、画像を描画
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