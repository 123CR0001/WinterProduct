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

	virtual void Draw(MATRIX mView = MGetIdent());

	void AddAnimation(class Animation* anim);
	void DeleteAnimation(class Animation* anim);

	std::vector<class Animation*> GetAnimations()& { return _anims; }

private:
	//ζΜnh
	int _handle;

	//ρ]AgεAΐW
	Transform2 _transform;

	//ζΜ`ζTCY
	Vector2 _size;

	//ζπ½]΅Δ`ζ·ι©
	bool _isTurn;

	//§Ύx 0.f~1.f ΜSͺ¦
	float _alpha;

	//_animsͺρ©
	bool _isPlayAnims;

	std::vector<class Animation*>_addAnims;
	std::vector<class Animation*>_deleteAnims;
	std::vector<class Animation*>_anims;
};