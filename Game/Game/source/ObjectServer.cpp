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
	//�Ǘ����Ă���I�u�W�F�N�g�����ׂč폜
	ClearObject();
	return true;
}

bool ObjectServer::Process() {

	//�I�u�W�F�N�g�����񏈗�
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
	//�I�u�W�F�N�g�����񏈗�

	for (int a = 0; a < _objects.size(); a++) {
		if (!_objects[a]->Render()) {
			return false;
		}
	}

	DrawFormatString(0, 600, GetColor(255, 0, 0), "�I�u�W�F�N�g�@%d",_objects.size());

	return true;
}

void ObjectServer::AddObject(ObjectBase* obj) {
	
	//���ɒǉ�����Ă��邩
	auto itr = std::find(_objects.begin(), _objects.end(), obj);
	if (itr != _objects.end()) {
		return;
	}
	//�܂��A�ǉ�����Ă��Ȃ������ŗ\��͓����Ă���̂ł́H
	itr = std::find(_addObj.begin(), _addObj.end(), obj);
	if (itr != _addObj.end()) {
		return;
	}

	obj->Initialize();
	_addObj.emplace_back(obj);
}

void ObjectServer::DeleteObject(ObjectBase* obj) {
	//���ɍ폜�\�񂳂�Ă��邩
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
	//���񏈗�������O�ɒǉ��ƍ폜�����Ă���
	for (auto&& addObj : _addObj) {
		//���ۂɒǉ�����Ă���A����������
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
	nlohmann::json j;
	std::string str = "res/stage/stage" + stage + "/" + area + "/StageData.json";
	std::ifstream file(str);

	if(!file) { return false; }
	file >> j;

	//�v���C���[�̓ǂݍ���
	for(auto&& object : j.at("player")) {
		std::string name = object.at("objectName");
		if(name == "marker1") {
			Player* p = NEW Player(this);
			p->SetJsonDataUE(object);
		}

	}

	//�G�l�~�[�̓ǂݍ���
	int count = 1;
	char num[10];

	std::string enemyName = "enemy_file/commonsoldier";

	while(1) {

		snprintf(num, 8, "%d", count);
		enemyName += num;

		if(j.find(enemyName.c_str()) != j.end()) {

			ObjectBase* p = NEW CommonSoldier(this);
			p->SetJsonDataUE(j.at(enemyName.c_str()));

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
		MV1RefreshCollInfo(p->GetHandle(), p->GetAttachIndex());
		return p;
		};

	std::unordered_map<std::string, ModelData>map;

	std::string strPath = "res/stage/stage" + stage + "/" + area + "/";

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

	{
		//�i�r�Q�[�V����
		if(j.find("navmesh") != j.end()) {
			for(auto&& navMesh : j.at("navmesh")) {
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

	//Spawner
	//NEW TracerSpawner(this);

	//�I�u�W�F�N�g�̔z�u
	for(auto&& object : j.at("object")) {
		std::string name = object.at("objectName");

		//�I�u�W�F�N�g���{����		��FEnemy1
		//�����������폜

		//0~9�̐������܂܂�Ă���΁A����ȍ~�̕������폜
		for(int a = 0; a < 10; a++) {
			//������string�ɕϊ��@����������
			int num = name.find(std::to_string(a));

			//find�́A���������������Ȃ���΁A-1��Ԃ�
			if(num != -1) { name = name.substr(0, num); break; }
		}

		if(map.find(name) != map.end()) {
			ObjectBase* p = map[name].func(map[name].filePath, map[name].attachFrameName);
			p->SetJsonDataUE(object);
			p->AddEulerAngle(Vector3D(DegToRad(90.f), DegToRad(180.f), 0.f));
			MV1RefreshCollInfo(p->GetHandle(), p->GetAttachIndex());
		}
	}


	return true;
}