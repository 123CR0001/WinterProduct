#include"EventLine.h"

EventLine::EventLine()
	:_frameCnt(0)
	,_isProcess(false)
{}

EventLine::~EventLine(){

	_eventFuncs.clear();
}

void EventLine::Process() {

	for(auto iter = _eventFuncs.begin(); iter != _eventFuncs.end(); ) {

		//イベントフラグが立った
		if((*iter).first()) {

			//イベントを処理する
			(*iter).second();

			iter = _eventFuncs.erase(iter);

		}
		else {
			++iter;
		}
	}

}