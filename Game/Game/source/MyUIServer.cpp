#include"MyUIServer.h"
#include"UI.h"
#include<algorithm>
#include"appframe.h"

MyUIServer::MyUIServer() 
{

}

MyUIServer::~MyUIServer(){
	for (int a = 0; a < _UIs.size(); a++) {
		delete _UIs[a];
	}
	_UIs.clear();
}

bool MyUIServer::Process() {
	if (!_addUIs.empty()) {
		for (int a = 0; a < _addUIs.size(); a++) {
			_UIs.emplace_back(_addUIs[a]);
		}
		_addUIs.clear();
	}

	if (!_deleteUIs.empty()) {
		for (int a = 0; a < _deleteUIs.size(); a++) {
			auto iter = std::find(_UIs.begin(), _UIs.end(), _deleteUIs[a]);

			if (iter != _UIs.end()) {
				_UIs.erase(iter);
			}
			delete _deleteUIs[a];
		}
		_deleteUIs.clear();
	}

	for (int a = 0; a < _UIs.size(); a++) {
		_UIs[a]->Process();
	}
	return true;
}

bool MyUIServer::Draw() {


	for (int a = 0; a < _UIs.size(); a++) {
		_UIs[a]->Draw();
	}

	return true;
}

void MyUIServer::AddUI(UI* addUI) {

	if (std::find(_addUIs.begin(), _addUIs.end(), addUI) != _addUIs.end()) { return; }
	if (std::find(_UIs.begin(), _UIs.end(), addUI) != _UIs.end()) { return; }

	_addUIs.emplace_back(addUI);
}

void MyUIServer::DeleteUI(UI* deleteUI) {

	if (std::find(_deleteUIs.begin(), _deleteUIs.end(), deleteUI) != _deleteUIs.end()) { return; }
	if (std::find(_UIs.begin(), _UIs.end(), deleteUI) == _UIs.end()) { return; }

	_deleteUIs.emplace_back(deleteUI);
}

UI* MyUIServer::Get(const char* name) {
	for(auto&& UI : _UIs) {
		if(UI->GetName() == name) {
			return UI;
		}
	}
	return nullptr;
}

