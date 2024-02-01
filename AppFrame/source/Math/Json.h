#pragma once
#include"../../../include/nlohmann/json.hpp"
#include<string>

class Json {
public:
	Json();
	virtual bool LoadData(std::string file_name);
	bool IsSuccess()const { return _success; }
	nlohmann::json GetJsonData()const { return _jsonData; }

protected:
	nlohmann::json _jsonData;
	//継承した後にコンストラクタでデータを読み込む際、成功したかどうか
	bool _success;
};