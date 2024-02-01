#include"Json.h"
#include<fstream>

Json::Json() {
	
}

bool Json::LoadData(std::string file_name) {
	std::ifstream file(file_name.c_str());
	if (!file) { return false; }
	//シリアライズ
	file >> _jsonData;
	return true;
}
