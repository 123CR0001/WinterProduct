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


ObjectBase::ObjectBase(ObjectServer* server, bool isFrame, std::string name)
	:_server(server)
	,_handle(0)
	,_attachIndex(0)
	,_name(name)
	,_state(STATE::kActive)
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

	ResourceServer::MV1DeleteModel(_handle);

	MV1TerminateCollInfo(_handle, _attachIndex);
}

bool ObjectBase::Initialize() {


	_scale = Vector3D(1.f, 1.f, 1.f);

	return true;
}

bool ObjectBase::Terminate() {


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

	ModelMatrixSetUp();
	return true;
}

bool ObjectBase::Render() {

	//�|���S���̗��ʂ�`�悵�Ȃ�
	MV1SetMeshBackCulling(_handle, 0, FALSE);

	ModelMatrixSetUp();

	// ���f����`�悷��
	MV1DrawModel(_handle);

	return true;
}

void ObjectBase::SetJsonDataUE(nlohmann::json j) {
	SetPos(Vector3D(j.at("translate").at("x"), j.at("translate").at("z"), -1 * j.at("translate").at("y")));
	SetEulerAngleDeg(Vector3D(j.at("rotate").at("x"), j.at("rotate").at("z"), j.at("rotate").at("y")));
	SetScale(Vector3D(j.at("scale").at("x"), j.at("scale").at("z"), j.at("scale").at("y")));
	ModelMatrixSetUp();
}

//���f���Ɋp�x�ƈړ��l�𔽉f������
void ObjectBase::ModelMatrixSetUp() {
	_matrix = MGetIdent();
	_matrix = MMult(_matrix, MGetScale(DxConverter::VecToDx(_scale)));
	_matrix = MMult(_matrix, MGetRotX(_eulerAngle.x));
	_matrix = MMult(_matrix, MGetRotZ(_eulerAngle.z));
	_matrix = MMult(_matrix, MGetRotY(_eulerAngle.y + PI));
	_matrix = MMult(_matrix, MGetTranslate(DxConverter::VecToDx(_pos)));
	MV1SetMatrix(_handle, _matrix);

	MV1RefreshCollInfo(_handle, _attachIndex);
}

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

void ObjectBase::DeleteComponent(Component* component) {
	//�܂��A�폜�\�񂳂�Ă��Ȃ�������A�폜�\�񂷂�
	auto iter = std::find(_deleteComponents.begin(), _deleteComponents.end(), component);
	if (iter == _deleteComponents.end()) {
		_deleteComponents.emplace_back(component);
	}
}

void ObjectBase::ApplyDamage(const DamageData& data) {
	_damageData = data;
}