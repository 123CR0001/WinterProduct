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

#include"MyUIServer.h"
#include"UIDetectionLevel.h"

#include<fstream>
#include<unordered_map>


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
		obj->Terminate();
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
			(*iter)->Terminate();
			delete (*iter);
			_objects.erase(iter);
		}
	}
	_deleteObj.clear();

	return true;
}

bool ObjectServer::LoadData(std::string stageName) {
	std::string stage = stageName.substr(0, 1);
	std::string area = stageName.substr(2, 1);
	nlohmann::json layoutJson;
	std::string str = "res/stage/stage" + stage + "/" + area + "/Layout.json";

	std::string strPath = "res/stage/stage" + stage + "/" + area + "/";

	std::ifstream layoutFile(str);

	if(!layoutFile) { return false; }
	layoutFile >> layoutJson;

	//ステージごとに異なるデータ(流すBGMやパラメーター)
	STAGE_DATA_ITEM item;
	{
		StageDataJson data(strPath + "StageData.json");
		if (data.IsSuccess()) {
			item = data.GetStageData();
		}
	}

	//プレイヤーの読み込み
	for(auto&& object : layoutJson.at("player")) {
		std::string name = object.at("objectName");
		if(name == "marker1") {
			Player* player = NEW Player(this);
			player->SetJsonDataUE(object);
			player->SetDecoyTimes(item.decoyTimes);
		}
	}

	//エネミーの読み込み
	int count = 1;
	char num[10];

	std::string enemyName = "enemy_file/commonsoldier";

	while(1) {

		snprintf(num, 8, "%d", count);
		enemyName += num;

		if(layoutJson.find(enemyName.c_str()) != layoutJson.end()) {

			ObjectBase* p = NEW CommonSoldier(this);
			p->SetJsonDataUE(layoutJson.at(enemyName.c_str()));

			enemyName = "enemy_file/commonsoldier";
			count++;

		}
		else {
			break;
		}
	}

	struct ModelData {
		const char* filePath;
		const char* attachFrameName;
		std::function < ObjectBase* (const char*, const char*) > func;
	};

	std::function < ObjectBase* (const char*, const char*) > func =
		[this](const char* path, const char* frameName) {
		ObjectBase* p = NEW ObjectBase(this, true);
		p->LoadModel(path, frameName);
		return p;
		};

	std::unordered_map<std::string, ModelData>map;



	std::string filePath = strPath + "model/StageObject.mv1";
	std::string attachFrameName = "UCX_StageObject";
	map["StageObject"].filePath = filePath.c_str();
	map["StageObject"].attachFrameName = attachFrameName.c_str();
	map["StageObject"].func = func;

	std::string filePathColl = strPath + "NavigationMesh.mv1";
	map["NavigationMesh"].filePath = filePathColl.c_str();
	map["NavigationMesh"].attachFrameName = "NavigationMesh";
	map["NavigationMesh"].func = func;

	map["siren"].filePath = "res/Object/siren/siren.mv1";
	map["siren"].attachFrameName = "UCX_siren1";
	map["siren"].func = [this](const char* path, const char* frameName) {return NEW Siren(this); };

	map["energy"].filePath = "";
	map["energy"].attachFrameName = "UCX_siren1";
	map["energy"].func = [this](const char* path, const char* frameName) {return NEW Energy(this); };

	map["tr_decoy"].filePath = "";
	map["tr_decoy"].attachFrameName = "";
	map["tr_decoy"].func = [this](const char* path, const char* frameName) {return NEW TutorialBox(this, "tr_decoy"); };

	map["tr_enemy"].filePath = "";
	map["tr_enemy"].attachFrameName = "";
	map["tr_enemy"].func = [this](const char* path, const char* frameName) {return NEW TutorialBox(this, "tr_enemy"); };

	map["tr_siren"].filePath = "";
	map["tr_siren"].attachFrameName = "";
	map["tr_siren"].func = [this](const char* path, const char* frameName) {return NEW TutorialBox(this, "tr_siren"); };

	map["tr_lightout"].filePath = "";
	map["tr_lightout"].attachFrameName = "";
	map["tr_lightout"].func = [this](const char* path, const char* frameName) {return NEW TutorialBox(this, "tr_lightout"); };

	map["tr_energy"].filePath = "";
	map["tr_energy"].attachFrameName = "";
	map["tr_energy"].func = [this](const char* path, const char* frameName) {return NEW TutorialBox(this, "tr_energy"); };

	{
		//ナビゲーション
		if(layoutJson.find("navmesh") != layoutJson.end()) {
			for(auto&& navMesh : layoutJson.at("navmesh")) {
				std::string name = navMesh.at("objectName");
				if(map.find(name) != map.end()) {

					_navi->LoadModel(map[name].filePath, map[name].attachFrameName);

					MV1SetPosition(_navi->GetHandle(), VGet(navMesh.at("translate").at("x"), navMesh.at("translate").at("z"), -1 * navMesh.at("translate").at("y")));
					MV1SetRotationXYZ(_navi->GetHandle(), VGet(DegToRad(90.f), 0.f, 0.f));
					MV1RefreshCollInfo(_navi->GetHandle(), _navi->GetAttachIndex());
					_navi->GetPolygonData();
					_navi->GetConectPolygonMap();


				}
			}
		}
	}

	//オブジェクトの配置
	for(auto&& object : layoutJson.at("object")) {
		std::string name = object.at("objectName");

		//オブジェクト名＋数字		例：Enemy1
		//数字部分を削除

		//0~9の数字が含まれていれば、それ以降の文字を削除
		for(int a = 0; a < 10; a++) {
			//整数をstringに変換　数字を検索
			int num = name.find(std::to_string(a));

			//findは、検索した文字がなければ、-1を返す
			if(num != -1) { name = name.substr(0, num); break; }
		}

		if(map.find(name) != map.end()) {
			ObjectBase* p = map[name].func(map[name].filePath, map[name].attachFrameName);
			p->SetJsonDataUE(object);
			p->AddEulerAngle(Vector3(DegToRad(90.f), DegToRad(180.f), 0.f));
			MV1RefreshCollInfo(p->GetHandle(), p->GetAttachIndex());
		}
	}

	return true;
}