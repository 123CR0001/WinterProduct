#pragma once
#include<unordered_map>
#include<functional>

class TimeLine {
public:
	TimeLine();
	~TimeLine();

	//フレーム数に対応した関数を追加
	void AddLine(int frameCnt, std::function<void()> func) { _funcs.emplace(_frameCnt + frameCnt, func); }

	void Process();
private:
	std::unordered_map<int, std::function<void()>>_funcs;		//フレーム数に対応した関数
	int _frameCnt;												//フレーム数	
};