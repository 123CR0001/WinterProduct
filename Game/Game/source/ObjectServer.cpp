#include"ObjectServer.h"
#include"Player.h"
#include"ModeGame.h"
#include<algorithm>
#include"PhysWorld.h"

#include"CommonSoldier.h"
#include"Player.h"
#include"Siren.h"
#include"Tracer.h"
#include"TracerSpawner.h"
#include"Energy.h"
#include"EventBox.h"
#include"TutorialBox.h"

#include"UIScreen.h"
#include"UIDetectionLevel.h"

#include<fstream>
#include<unordered_map>

#include"ClearData.h"


ObjectServer::ObjectServer(ModeGame* game)
:_game(game)
,_physWorld(NEW PhysWorld(this))
,_navi(NEW Navi())
{
	Initialize();
}

ObjectServer::~ObjectServer() {
	Terminate();
	delete _physWorld;
	delete _navi;
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

	//巡回処理をする前にオブジェクトの追加と削除をしておく
	if(!_addObj.empty()) {
		for(auto&& addObj : _addObj) {
			//実際に追加されてから、初期化する
			addObj->Initialize();
			_objects.emplace_back(addObj);
		}
		_addObj.clear();
	}
	
	if(!_deleteObj.empty()){
		for(auto&& deleteObj : _deleteObj) {
			auto iter = std::find(_objects.begin(), _objects.end(), deleteObj);
			if(iter != _objects.end()) {
				(*iter)->Terminate();
				delete (*iter);
				_objects.erase(iter);
			}
		}
		_deleteObj.clear();
	}

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
		if (!_objects[a]->Render()) {
			return false;
		}
	}


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

	//初期化
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
		obj->Terminate();
		delete obj;
	}
	_addObj.clear();

	_deleteObj.clear();
	return true;
}

bool ObjectServer::LoadData(std::string stageName) {
	std::string stage = stageName.substr(0, 1);
	std::string area = stageName.substr(2, 1);

	//ファイルのパス
	std::string filePath = "res/stage/stage" + stage + "/" + area + "/";

	//ファイルの読み込み
	std::ifstream layoutFile(filePath + "Layout.json");

	//Json
	nlohmann::json layoutJson;

	//シリアライズ
	if(!layoutFile) { return false; }
	layoutFile >> layoutJson;

	//ステージごとに異なるデータ(流すBGMやパラメーター)
	STAGE_DATA_ITEM item;
	{
		StageDataJson data(filePath + "StageData.json");
		if (data.IsSuccess()) {
			item = data.GetStageData();
			_game->GetClearData()->_nextStageName = item.nextStageName;
		}
	}

	std::unordered_map<std::string, std::function <ObjectBase*()>>map;


	map["StageObject"] =
		[=]()mutable {
		ObjectBase* p = NEW ObjectBase(this, true);
		p->LoadModel(filePath + "model/StageObject.mv1", "UCX_StageObject");
		return p;
	};;
	map["siren"] = [this]() {return NEW Siren(this); };
	map["energy"] = [this]() {return NEW Energy(this); };
	map["tr_decoy"] = [this]() {return NEW TutorialBox(this, "tr_decoy"); };
	map["tr_enemy"] = [this]() {return NEW TutorialBox(this, "tr_enemy"); };
	map["tr_siren"] = [this]() {return NEW TutorialBox(this, "tr_siren"); };
	map["tr_lightout"] = [this]() {return NEW TutorialBox(this, "tr_lightout"); };
	map["tr_energy"] = [this]() {return NEW TutorialBox(this, "tr_energy"); };

	
	//ナビゲーション
	if(layoutJson.find("navmesh") != layoutJson.end()) {
		for(auto&& navMesh : layoutJson.at("navmesh")) {

			auto fileName = filePath + "NavigationMesh.mv1";

			_navi->LoadModel(fileName.c_str());

			MV1SetPosition(_navi->GetHandle(), VGet(navMesh.at("translate").at("x"), navMesh.at("translate").at("z"), -1 * navMesh.at("translate").at("y")));
			MV1SetRotationXYZ(_navi->GetHandle(), VGet(DegToRad(90.f), 0.f, 0.f));
			MV1RefreshCollInfo(_navi->GetHandle(),0);

			_navi->CreateNavigationMesh();	
			
		}
	}

	//オブジェクトの配置
	for(auto&& object : layoutJson.at("object")) {
		std::string name = object.at("objectName");
		if(map.find(name) != map.end()) {
			auto p = map[name]();
			p->SetJsonDataUE(object);
			p->AddEulerAngle(Vector3(DegToRad(90.f), DegToRad(180.f), 0.f));
		}
	}


	//プレイヤーの読み込み
	for (auto&& object : layoutJson.at("player")) {
		std::string name = object.at("objectName");
		if (name == "marker1") {
			Player* player = NEW Player(this);
			player->SetJsonDataUE(object);
			player->SetDecoyTimes(item.decoyTimes);
		}
	}

	//エネミーの読み込み
	int count = 1;
	char num[10];

	std::string enemyName = "enemy_file/commonsoldier";

	while (1) {

		snprintf(num, 8, "%d", count);
		enemyName += num;

		if (layoutJson.find(enemyName.c_str()) != layoutJson.end()) {

			ObjectBase* p = NEW CommonSoldier(this);
			p->SetJsonDataUE(layoutJson.at(enemyName.c_str()));

			enemyName = "enemy_file/commonsoldier";
			count++;

		}
		else {
			break;
		}
	}

	
	return true;
}