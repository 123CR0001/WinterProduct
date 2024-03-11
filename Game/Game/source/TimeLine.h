#pragma once
#include<unordered_map>
#include<functional>

class TimeLine {
public:
	TimeLine();
	~TimeLine();

	void AddLine(int frameCnt, std::function<void()> func) { _funcs.emplace(_frameCnt + frameCnt, func); }

	bool Process();
private:
	std::unordered_map<int, std::function<void()>>_funcs;
	int _frameCnt;
};