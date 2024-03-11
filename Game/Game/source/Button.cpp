#include"Button.h"
#include"ButtonServer.h"

Button::Button(ButtonServer* server, std::function<void()> selectFunc,SpriteText* text)
	:_server(server)
	,_selectFunc(selectFunc)
	,_text(text)
{

}

Button::~Button(){
	delete _text;
}


bool Button::Process() {
	return true;
}

bool Button::Draw() {
	_text->Draw();
	return true;
}