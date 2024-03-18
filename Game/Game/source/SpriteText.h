#pragma once
#include"appframe.h"

class SpriteText {
public:
	SpriteText();
	SpriteText(int handle, const Transform2& tarnsform, const Vector2& size, float alpha = 1.f);
	virtual ~SpriteText();

	void SetHandle(int handle) { _handle = handle; }
	void SetTransform(const Transform2& transform) { _transform = transform; }
	void SetPos(const Vector2& pos) { _transform.pos = pos; }
	void SetRad(float rad) { _transform.rad = rad; }
	void SetZoom(float zoom) { _transform.zoom = zoom; }
	void SetSize(const Vector2& size) { _size = size;}
	void SetAlpha(float alpha) { _alpha = alpha; }


	int GetHandle()const { return _handle; }
	const Transform2& GetTransform()const { return _transform; }
	const Vector2& GetPos()const { return _transform.pos; }
	float GetRad()const { return _transform.rad; }
	float GetZoom()const { return _transform.zoom; }
	const Vector2& GetSize()const { return _size; }
	float GetAlpha()const { return _alpha; }

	void Turn() { _isTurn = !_isTurn; }

	void Reverse();

	virtual void Draw(MATRIX mView = MGetIdent());

	void AddAnimation(class Animation* anim);
	void DeleteAnimation(class Animation* anim);

	std::vector<class Animation*> GetAnimations()& { return _anims; }

	bool IsAllAnimationEnd()const { return _isAllAniamtionEnd; }

private:
	//画像のハンドル
	int _handle;

	//回転、拡大、座標
	Transform2 _transform;

	//画像の描画サイズ
	Vector2 _size;

	//画像を反転して描画するか
	bool _isTurn;

	//透明度 0.f~1.f の百分率
	float _alpha;

	//_animsが巡回中か
	bool _isPlayAnims;

	//保持するアニメーションが全て終わったか
	bool _isAllAniamtionEnd;

	std::vector<class Animation*>_addAnims;
	std::vector<class Animation*>_deleteAnims;
	std::vector<class Animation*>_anims;
};