#include"SpriteText.h"
#include"Animation.h"

#include<algorithm>
SpriteText::SpriteText()
	:_handle(0)
	, _isTurn(false)
	,_alpha(1.f)
	,_isPlayAnims(false)
{

}

SpriteText::SpriteText(int handle, const Transform2& transform, const Vector2& size, float alpha) 
	:_handle(handle)
	,_transform(transform)
	,_size(size)
	,_alpha(alpha)
	,_isTurn(false)
	,_isPlayAnims(false)
{

}

SpriteText::~SpriteText() {

	for (auto&& anim : _anims) {
		delete anim;
	}

	_anims.clear();
}

void SpriteText::Draw(MATRIX mView) {

	if (!_addAnims.empty()) {
		for (int a = 0; a < _addAnims.size(); a++) {
			_anims.emplace_back(_addAnims[a]);
		}
		_addAnims.clear();
	}

	if (!_deleteAnims.empty()) {
		for (int a = 0; a < _deleteAnims.size(); a++) {
			auto iter = std::find(_anims.begin(), _anims.end(), _deleteAnims[a]);

			_anims.erase(iter);

			delete _deleteAnims[a];
		}

		_deleteAnims.clear();
	}

	_isPlayAnims = true;
	for (int a = 0; a < _anims.size(); a++) {
		_anims[a]->Process();
	}
	_isPlayAnims = false;

	// 回転×拡縮×並行移動

	MATRIX mRot = MGetRotZ(_transform.rad);						// 2Dでx,yを回転させるには、3DならZ軸回転になる
	MATRIX mScale = MGetScale(VGet(_transform.zoom, _transform.zoom, 1));		// Z方向の倍率は1のまま
	MATRIX mTrans = MGetTranslate(VGet(_transform.pos.x, _transform.pos.y, 0.f));
	MATRIX m;
	m = MMult(mRot, mScale);
	m = MMult(m, mTrans);

	m = MMult(m, mView);

	const float width = _size.x;
	const float height = _size.y;

	// 描画する画像の4つの頂点座標
	VECTOR _pos[4] = {
		// 通常用（左上から右回り）
		VGet(-width / 2.0, -height / 2.0, 0),	// 左上
		VGet(width / 2.0, -height / 2.0 ,0),	// 右上
		VGet(width / 2.0,  height / 2.0 ,0),	// 右下
		VGet(-width / 2.0,  height / 2.0 ,0),	// 左下
	};

	if(_isTurn) {
		// 反転用
		VECTOR posTurn[4] = {
			// 反転用（右上から左回り）
			VGet(width / 2.0, -height / 2.0 ,0),	// 右上
			VGet(-width / 2.0, -height / 2.0, 0),	// 左上
			VGet(-width / 2.0,  height / 2.0 ,0),	// 左下
			VGet(width / 2.0,  height / 2.0 ,0),	// 右下
		};
		for(int i = 0; i < 4; i++) {
			_pos[i] = posTurn[i];
		}
	}

	// 4つの頂点座標全てに行列を掛けて変換する
	for(int i = 0; i < 4; i++) {
		_pos[i] = VTransform(_pos[i], m);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * _alpha));

	// 変換した座標で描画する
	DrawModiGraphF(
		_pos[0].x, _pos[0].y,
		_pos[1].x, _pos[1].y,
		_pos[2].x, _pos[2].y,
		_pos[3].x, _pos[3].y,
		_handle, true
	);

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SpriteText::AddAnimation(Animation* anim) {

	//すでに追加予約されているか
	if (std::find(_addAnims.begin(), _addAnims.end(), anim) != _addAnims.end()) { return; }

	//すでに追加されているか
	if (std::find(_anims.begin(), _anims.end(), anim) != _anims.end()) { return; }

	//_animsが巡回中でなければ、直接追加
	if (!_isPlayAnims) {
		_anims.emplace_back(anim);
	}
	else {
		//追加予約
		_addAnims.emplace_back(anim);
	}
}

void SpriteText::DeleteAnimation(Animation* anim) {

	//すでに削除予約されているか
	if (std::find(_deleteAnims.begin(), _deleteAnims.end(), anim) != _deleteAnims.end()) { return; }

	//そもそもあるのか
	auto iter = std::find(_anims.begin(), _anims.end(), anim);
	
	if(iter ==_anims.end()) { return; }

	//_animsが巡回中でなければ、直接削除
	if (!_isPlayAnims) {
		_anims.erase(iter);
	}
	else {
		//削除予約
		_deleteAnims.emplace_back(anim);
	}

}