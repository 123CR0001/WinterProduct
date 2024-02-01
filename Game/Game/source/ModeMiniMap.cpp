#include"ModeMiniMap.h"
#include"ModeGame.h"
ModeMiniMap::ModeMiniMap(ModeGame* game) {
	_game = game;
}

ModeMiniMap::~ModeMiniMap() {

}

bool ModeMiniMap::Initialize() {
	return true;
}

bool ModeMiniMap::Process() {
	return true;
}

bool ModeMiniMap::Render() {
	return true;
}