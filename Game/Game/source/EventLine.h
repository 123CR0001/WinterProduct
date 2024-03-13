#pragma once
#include<vector>
#include<utility>
#include<functional>

using EventFlag = std::function<bool()>;
using Event = std::function<void()>;

class EventLine {
public:
	EventLine();
	~EventLine();

	void Process();

	void AddLine(EventFlag flag, Event event) {
		
		if(!_isProcess) {
			_eventFuncs.emplace_back(std::make_pair(flag, event));
		}
		else {
			_addEventFuncs.emplace_back(flag, event);
		}
	}

private:

	int _frameCnt;
	std::vector<std::pair<EventFlag, Event>>_eventFuncs;
	std::vector<std::pair<EventFlag, Event>>_addEventFuncs;

	bool _isProcess;
};