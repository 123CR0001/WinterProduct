#include"EventLine.h"

EventLine::EventLine(){}

EventLine::~EventLine(){

	_eventFuncs.clear();
}

void EventLine::Process() {

	for(auto iter = _eventFuncs.begin(); iter != _eventFuncs.end(); ) {

		//�C�x���g�t���O��������
		if((*iter).first()) {

			//�C�x���g����������
			(*iter).second();

			iter = _eventFuncs.erase(iter);

		}
		else {
			++iter;
		}
	}

}