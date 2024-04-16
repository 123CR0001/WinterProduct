#pragma once
#include"appframe.h"

class SpriteText {
public:
	SpriteText();
	SpriteText(int handle, const Transform2& tarnsform, const Vector2& size, float alpha = 1.f);
	virtual ~SpriteText();

	//�摜�̃n���h���A���W�A��]�A�g��A�T�C�Y�A�����x��ݒ�
	void SetHandle(int handle) { _handle = handle; }
	void SetTransform(const Transform2& transform) { _transform = transform; }
	void SetPos(const Vector2& pos) { _transform.pos = pos; }
	void SetRad(float rad) { _transform.rad = rad; }
	void SetZoom(float zoom) { _transform.zoom = zoom; }
	void SetSize(const Vector2& size) { _size = size;}
	void SetAlpha(float alpha) { _alpha = alpha; }

	//�摜�̃n���h���A���W�A��]�A�g��A�T�C�Y�A�����x���擾
	int GetHandle()const { return _handle; }
	const Transform2& GetTransform()const { return _transform; }
	const Vector2& GetPos()const { return _transform.pos; }
	float GetRad()const { return _transform.rad; }
	float GetZoom()const { return _transform.zoom; }
	const Vector2& GetSize()const { return _size; }
	float GetAlpha()const { return _alpha; }

	//�摜�𔽓]���ĕ`�悷�邩
	void Turn() { _isTurn = !_isTurn; }

	//�A�j���[�V������S�ċt�Đ�����
	void Reverse();

	virtual void Draw(MATRIX mView = MGetIdent());

	//�A�j���[�V�����̒ǉ�
	void AddAnimation(class Animation* anim);

	//�A�j���[�V�����̍폜
	void DeleteAnimation(class Animation* anim);

	//�A�j���[�V�����̃��X�g���擾
	std::vector<class Animation*> GetAnimations()& { return _anims; }

	//�S�ẴA�j���[�V�������I�������
	bool IsAllAnimationEnd()const { return _isAllAniamtionEnd; }

private:

	int _handle;					//�摜�̃n���h��

	Transform2 _transform;			//�摜�̍��W�A��]�A�g��

	Vector2 _size;					//�摜�̃T�C�Y	

	bool _isTurn;					//�摜�𔽓]���ĕ`�悷�邩	

	float _alpha;					//�摜�̓����x	

	bool _isPlayAnims;				//_anims�����񒆂�

	bool _isAllAniamtionEnd;		//�ێ�����A�j���[�V�������S�ďI�������

	std::vector<class Animation*>_addAnims;
	std::vector<class Animation*>_deleteAnims;
	std::vector<class Animation*>_anims;
};