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
	//�p��������ɃR���X�g���N�^�Ńf�[�^��ǂݍ��ލہA�����������ǂ���
	bool _success;
};