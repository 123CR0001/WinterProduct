#include"ObjectBase.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include<string>
#include<algorithm>
#include <iterator>
#include<utility>
#include"Component.h"
#include"PhysWorld.h"
#include"FrameComponent.h"
#include"CapsuleComponent.h"

/*
���ʉ��ł������ȏ����́A�Ϗ��^�őΉ�����
*/

ObjectBase::ObjectBase(ObjectServer* server, bool isFrame, std::string name)
	:_server(server)
	,_handle(0)
	,_attachIndex(0)
	,_name(name)
	,_state(STATE::kActive)
	,_scale(Vector3(1.f, 1.f, 1.f))
{
	//�������Ǘ�����T�[�o�[���L�^����
	_server->AddObject(this);

	if (isFrame) { NEW FrameComponent(this); }

}

ObjectBase::~ObjectBase() {

	for (auto iter = _components.begin(); iter != _components.end(); ++iter) {
		delete (*iter);
	}

	for (int a = 0; a < _addComponents.size(); a++) {
		delete _addComponents[a];
	}

	_addComponents.clear();
	_deleteComponents.clear();
	_components.clear();

}

bool ObjectBase::Initialize() {
	ModelMatrixSetUp();
	return true;
}

bool ObjectBase::Terminate() {

	ResourceServer::MV1DeleteModel(_handle);
	return true;
}

bool ObjectBase::Process() {
	//�R���|�[�l���g�̒ǉ�
	if(_addComponents.size() > 0) {
		for(auto&& add : _addComponents) {
			_components.emplace_back(add);
		}
		_addComponents.clear();

		//�~���Ƀ\�[�g
		for(int a = 0; a < _components.size(); a++) {
			for(int b = a; b < _components.size(); b++) {
				if(_components[a]->GetOrder() > _components[b]->GetOrder()) {
					auto temp = _components[a];
					_components[a] = _components[b];
					_components[b] = temp;
				}
			}
		}
	}

	//�R���|�[�l���g�̍폜
	if (_deleteComponents.size() > 0) {
		for (auto&& del : _deleteComponents) {
			auto iter = std::find(_components.begin(), _components.end(), del);
			if (iter != _components.end()) {
				delete (*iter);
				_components.erase(iter);
			}
		}
		_deleteComponents.clear();
	}

	for (auto iter = _components.begin(); iter != _components.end(); ++iter) {
		(*iter)->Process();
	}

	//���̏������f���ɔ��f������
	ModelMatrixSetUp();
	return true;
}

bool ObjectBase::Render() {

	// ���f����`�悷��
	MV1DrawModel(_handle);

	return true;
}

//���W�Ȃǂ̓ǂݍ���
void ObjectBase::SetJsonDataUE(nlohmann::json j) {
	SetPos(Vector3(j.at("translate").at("x"), j.at("translate").at("z"), -1 * j.at("translate").at("y")));
	SetEulerAngleDeg(Vector3(j.at("rotate").at("x"), j.at("rotate").at("z"), j.at("rotate").at("y")));
	SetScale(Vector3(j.at("scale").at("x"), j.at("scale").at("z"), j.at("scale").at("y")));
	ModelMatrixSetUp();
}

//���f���Ɋp�x�ƈړ��l�𔽉f������
void ObjectBase::ModelMatrixSetUp() {
	MATRIX matrix = MGetIdent();
	matrix = MMult(matrix, MGetScale(DxConverter::VecToDx(_scale)));
	matrix = MMult(matrix, MGetRotX(_eulerAngle.x));
	matrix = MMult(matrix, MGetRotZ(_eulerAngle.z));
	matrix = MMult(matrix, MGetRotY(_eulerAngle.y + PI));
	matrix = MMult(matrix, MGetTranslate(DxConverter::VecToDx(_pos)));
	MV1SetMatrix(_handle, matrix);

	MV1RefreshCollInfo(_handle, _attachIndex);
}

//���f���̓ǂݍ���
bool ObjectBase::LoadModel(std::string FileName ,std::string attachFrameName) {

	_handle = ResourceServer::MV1LoadModel(FileName.c_str());

	if (_handle < 0) {
		return false;
	}

	if (attachFrameName.size() > 0) {
		_attachIndex = MV1SearchFrame(_handle, attachFrameName.c_str());
		MV1SetFrameVisible(_handle, _attachIndex, FALSE);
	}
	return true;
}

//�R���|�[�l���g�̒ǉ�
void ObjectBase::AddComponent(Component* component) {

	//���ɒǉ�����ĂȂ���
	auto iter = std::find(_components.begin(), _components.end(), component);
	if (iter != _components.end()) { return; }

	//���ɒǉ��\�񂳂�Ă��Ȃ���
	iter = std::find(_addComponents.begin(), _addComponents.end(), component);
	if (iter != _addComponents.end()) { return; }

	//���ɍ폜�\�񂳂�Ă��Ȃ���
	iter = std::find(_deleteComponents.begin(), _deleteComponents.end(), component);
	if (iter != _deleteComponents.end()) { return; }

	//�ǉ��\��
	_addComponents.emplace_back(component);
}

//�R���|�[�l���g�̍폜
void ObjectBase::DeleteComponent(Component* component) {
	//�܂��A�폜�\�񂳂�Ă��Ȃ�������A�폜�\�񂷂�
	auto iter = std::find(_deleteComponents.begin(), _deleteComponents.end(), component);
	if (iter == _deleteComponents.end()) {
		_deleteComponents.emplace_back(component);
	}
}

//�_���[�W
void ObjectBase::ApplyDamage(const DamageData& data) {
	_damageData = data;
}