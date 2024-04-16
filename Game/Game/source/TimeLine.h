#pragma once
#include<unordered_map>
#include<functional>

class TimeLine {
public:
	TimeLine();
	~TimeLine();

	//�t���[�����ɑΉ������֐���ǉ�
	void AddLine(int frameCnt, std::function<void()> func) { _funcs.emplace(_frameCnt + frameCnt, func); }

	void Process();
private:
	std::unordered_map<int, std::function<void()>>_funcs;		//�t���[�����ɑΉ������֐�
	int _frameCnt;												//�t���[����	
};