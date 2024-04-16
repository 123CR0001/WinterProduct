#pragma once
#include"appframe.h"

class SpriteText {
public:
	SpriteText();
	SpriteText(int handle, const Transform2& tarnsform, const Vector2& size, float alpha = 1.f);
	virtual ~SpriteText();

	//画像のハンドル、座標、回転、拡大、サイズ、透明度を設定
	void SetHandle(int handle) { _handle = handle; }
	void SetTransform(const Transform2& transform) { _transform = transform; }
	void SetPos(const Vector2& pos) { _transform.pos = pos; }
	void SetRad(float rad) { _transform.rad = rad; }
	void SetZoom(float zoom) { _transform.zoom = zoom; }
	void SetSize(const Vector2& size) { _size = size;}
	void SetAlpha(float alpha) { _alpha = alpha; }

	//画像のハンドル、座標、回転、拡大、サイズ、透明度を取得
	int GetHandle()const { return _handle; }
	const Transform2& GetTransform()const { return _transform; }
	const Vector2& GetPos()const { return _transform.pos; }
	float GetRad()const { return _transform.rad; }
	float GetZoom()const { return _transform.zoom; }
	const Vector2& GetSize()const { return _size; }
	float GetAlpha()const { return _alpha; }

	//画像を反転して描画するか
	void Turn() { _isTurn = !_isTurn; }

	//アニメーションを全て逆再生する
	void Reverse();

	virtual void Draw(MATRIX mView = MGetIdent());

	//アニメーションの追加
	void AddAnimation(class Animation* anim);

	//アニメーションの削除
	void DeleteAnimation(class Animation* anim);

	//アニメーションのリストを取得
	std::vector<class Animation*> GetAnimations()& { return _anims; }

	//全てのアニメーションが終わったか
	bool IsAllAnimationEnd()const { return _isAllAniamtionEnd; }

private:

	int _handle;					//画像のハンドル

	Transform2 _transform;			//画像の座標、回転、拡大

	Vector2 _size;					//画像のサイズ	

	bool _isTurn;					//画像を反転して描画するか	

	float _alpha;					//画像の透明度	

	bool _isPlayAnims;				//_animsが巡回中か

	bool _isAllAniamtionEnd;		//保持するアニメーションが全て終わったか

	std::vector<class Animation*>_addAnims;
	std::vector<class Animation*>_deleteAnims;
	std::vector<class Animation*>_anims;
};