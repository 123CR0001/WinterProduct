#include"ObjectServer.h"
#include"Player.h"
#include"ModeGame.h"
#include<algorithm>
#include"PhysWorld.h"


ObjectServer::ObjectServer(ModeGame* game)
:_game(game)
,_physWorld(new PhysWorld(this))
{
	Initialize();
}

ObjectServer::~ObjectServer() {

	Terminate();
}

bool ObjectServer::Initialize() {

	return true;
}

bool ObjectServer::Terminate() {
	//管理しているオブジェクトをすべて削除
	ClearObject();
	return true;
}

bool ObjectServer::Process() {

	//オブジェクトを巡回処理
	for (auto iter = _objects.begin(); iter != _objects.end();++iter) {
		if ((*iter)->Process()) {
			
		}
		else {
			return false;
		}
	}
	return true;
}

bool ObjectServer::Renderer() {
	//オブジェクトを巡回処理

	for (int a = 0; a < _objects.size(); a++) {
		ChangeLightTypePoint(VAdd(_objects[a]->GetDxPos(), VGet(0.f, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);
		if (!_objects[a]->Render()) {
			return false;
		}
	}

	DrawFormatString(0, 600, GetColor(255, 0, 0), "オブジェクト　%d",_objects.size());

	return true;
}

void ObjectServer::AddObject(ObjectBase* obj) {
	
	//既に追加されているか
	auto itr = std::find(_objects.begin(), _objects.end(), obj);
	if (itr != _objects.end()) {
		return;
	}
	//まだ、追加されていないだけで予約は入っているのでは？
	itr = std::find(_addObj.begin(), _addObj.end(), obj);
	if (itr != _addObj.end()) {
		return;
	}

	obj->Initialize();
	_addObj.emplace_back(obj);
}

void ObjectServer::DeleteObject(ObjectBase* obj) {
	//既に削除予約されているか
	auto iter = std::find(_deleteObj.begin(), _deleteObj.end(), obj);
	if (iter != _deleteObj.end()) {
		return;
	}

	iter = std::find(_addObj.begin(), _addObj.end(), obj);
	if (iter != _addObj.end()) {
		(*iter)->Terminate();
		delete (*iter);
		_addObj.erase(iter);
		return;
	}
	_deleteObj.emplace_back(obj);
}

bool ObjectServer::ClearObject() {

	for (auto&& obj : _objects) {
		delete obj;
	}
	_objects.clear();

	for (auto&& obj : _addObj) {
		delete obj;
	}
	_addObj.clear();

	_deleteObj.clear();
	return true;
}

bool ObjectServer::ProcessInit() {
	//巡回処理をする前に追加と削除をしておく
	for (auto&& addObj : _addObj) {
		//実際に追加されてから、初期化する
		addObj->Initialize();
		_objects.emplace_back(addObj);
	}
	_addObj.clear();

	for (auto&& deleteObj : _deleteObj) {
		auto iter = std::find(_objects.begin(), _objects.end(), deleteObj);
		if (iter != _objects.end()) {
			delete (*iter);
			_objects.erase(iter);
		}
	}
	_deleteObj.clear();

	return true;
}