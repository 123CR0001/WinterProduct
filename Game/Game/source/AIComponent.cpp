#include"AIComponent.h"
#include"AIState.h"

AIComponent::AIComponent(ObjectBase* owner,int order)
	:Component(owner,order)
,_currentState(nullptr)
{}
AIComponent::~AIComponent(){
	for (auto& state : _stateMap) {
		delete state.second;
	}

	_stateMap.clear();
}

bool AIComponent::Process() {

	if (_currentState) {
		_currentState->Process();
	}
	return true;
}

bool AIComponent::ChangeState(const std::string& name) {
	if (_currentState) {
		_currentState->OnExist();
	}

	auto iter = _stateMap.find(name);

	if (iter != _stateMap.end()) {
		_currentState = iter->second;
		_currentState->OnEnter();
		return true;
	}
	else { _currentState = nullptr; }
	return false;
}

void AIComponent::RegisterState(AIState* state) {
	_stateMap.emplace(state->GetName(), state);
}

void AIComponent::AddPoint(const std::string& name, const Vector3D& point) {
	_points[name].emplace_back(point);
}

void AIComponent::InsertPoint(const std::string& name, const Vector3D& point,const int num) {
	_points[name].insert(_points[name].begin() + num, point);
}

void AIComponent::DeletePoint(const std::string& name, int num) {
	//numに0以上の値が入れられた場合は、その番号のコンテナを削除
	if (num > 0) {
		_points[name].erase(_points[name].begin() + num);
		return;
	}
	//入れられなかったら、全削除
	_points[name].clear();
}