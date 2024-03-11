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

	// ��]�~�g�k�~���s�ړ�

	MATRIX mRot = MGetRotZ(_transform.rad);						// 2D��x,y����]������ɂ́A3D�Ȃ�Z����]�ɂȂ�
	MATRIX mScale = MGetScale(VGet(_transform.zoom, _transform.zoom, 1));		// Z�����̔{����1�̂܂�
	MATRIX mTrans = MGetTranslate(VGet(_transform.pos.x, _transform.pos.y, 0.f));
	MATRIX m;
	m = MMult(mRot, mScale);
	m = MMult(m, mTrans);

	m = MMult(m, mView);

	const float width = _size.x;
	const float height = _size.y;

	// �`�悷��摜��4�̒��_���W
	VECTOR _pos[4] = {
		// �ʏ�p�i���ォ��E���j
		VGet(-width / 2.0, -height / 2.0, 0),	// ����
		VGet(width / 2.0, -height / 2.0 ,0),	// �E��
		VGet(width / 2.0,  height / 2.0 ,0),	// �E��
		VGet(-width / 2.0,  height / 2.0 ,0),	// ����
	};

	if(_isTurn) {
		// ���]�p
		VECTOR posTurn[4] = {
			// ���]�p�i�E�ォ�獶���j
			VGet(width / 2.0, -height / 2.0 ,0),	// �E��
			VGet(-width / 2.0, -height / 2.0, 0),	// ����
			VGet(-width / 2.0,  height / 2.0 ,0),	// ����
			VGet(width / 2.0,  height / 2.0 ,0),	// �E��
		};
		for(int i = 0; i < 4; i++) {
			_pos[i] = posTurn[i];
		}
	}

	// 4�̒��_���W�S�Ăɍs����|���ĕϊ�����
	for(int i = 0; i < 4; i++) {
		_pos[i] = VTransform(_pos[i], m);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * _alpha));

	// �ϊ��������W�ŕ`�悷��
	DrawModiGraphF(
		_pos[0].x, _pos[0].y,
		_pos[1].x, _pos[1].y,
		_pos[2].x, _pos[2].y,
		_pos[3].x, _pos[3].y,
		_handle, true
	);

	// �`��u�����h���[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SpriteText::AddAnimation(Animation* anim) {

	//���łɒǉ��\�񂳂�Ă��邩
	if (std::find(_addAnims.begin(), _addAnims.end(), anim) != _addAnims.end()) { return; }

	//���łɒǉ�����Ă��邩
	if (std::find(_anims.begin(), _anims.end(), anim) != _anims.end()) { return; }

	//_anims�����񒆂łȂ���΁A���ڒǉ�
	if (!_isPlayAnims) {
		_anims.emplace_back(anim);
	}
	else {
		//�ǉ��\��
		_addAnims.emplace_back(anim);
	}
}

void SpriteText::DeleteAnimation(Animation* anim) {

	//���łɍ폜�\�񂳂�Ă��邩
	if (std::find(_deleteAnims.begin(), _deleteAnims.end(), anim) != _deleteAnims.end()) { return; }

	//������������̂�
	auto iter = std::find(_anims.begin(), _anims.end(), anim);
	
	if(iter ==_anims.end()) { return; }

	//_anims�����񒆂łȂ���΁A���ڍ폜
	if (!_isPlayAnims) {
		_anims.erase(iter);
	}
	else {
		//�폜�\��
		_deleteAnims.emplace_back(anim);
	}

}