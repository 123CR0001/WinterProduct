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
共通化できそうな処理は、委譲型で対応する
*/

ObjectBase::ObjectBase(ObjectServer* server, bool isFrame, std::string name)
	:_server(server)
	,_handle(0)
	,_attachIndex(0)
	,_name(name)
	,_state(STATE::kActive)
	,_scale(Vector3(1.f, 1.f, 1.f))
{
	//自分を管理するサーバーを記録する
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
	//コンポーネントの追加
	if(_addComponents.size() > 0) {
		for(auto&& add : _addComponents) {
			_components.emplace_back(add);
		}
		_addComponents.clear();

		//降順にソート
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

	//コンポーネントの削除
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

	//今の情報をモデルに反映させる
	ModelMatrixSetUp();
	return true;
}

bool ObjectBase::Render() {

	// モデルを描画する
	MV1DrawModel(_handle);

	return true;
}

//座標などの読み込み
void ObjectBase::SetJsonDataUE(nlohmann::json j) {
	SetPos(Vector3(j.at("translate").at("x"), j.at("translate").at("z"), -1 * j.at("translate").at("y")));
	SetEulerAngleDeg(Vector3(j.at("rotate").at("x"), j.at("rotate").at("z"), j.at("rotate").at("y")));
	SetScale(Vector3(j.at("scale").at("x"), j.at("scale").at("z"), j.at("scale").at("y")));
	ModelMatrixSetUp();
}

//モデルに角度と移動値を反映させる
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

//モデルの読み込み
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

//コンポーネントの追加
void ObjectBase::AddComponent(Component* component) {

	//既に追加されてないか
	auto iter = std::find(_components.begin(), _components.end(), component);
	if (iter != _components.end()) { return; }

	//既に追加予約されていないか
	iter = std::find(_addComponents.begin(), _addComponents.end(), component);
	if (iter != _addComponents.end()) { return; }

	//既に削除予約されていないか
	iter = std::find(_deleteComponents.begin(), _deleteComponents.end(), component);
	if (iter != _deleteComponents.end()) { return; }

	//追加予約
	_addComponents.emplace_back(component);
}

//コンポーネントの削除
void ObjectBase::DeleteComponent(Component* component) {
	//まだ、削除予約されていなかったら、削除予約する
	auto iter = std::find(_deleteComponents.begin(), _deleteComponents.end(), component);
	if (iter == _deleteComponents.end()) {
		_deleteComponents.emplace_back(component);
	}
}

//ダメージ
void ObjectBase::ApplyDamage(const DamageData& data) {
	_damageData = data;
}