#include"UISpriteText.h"
#include"SpriteText.h"

UISpriteText::UISpriteText(SpriteText* text)
	:_text(text)
{

}

UISpriteText::~UISpriteText(){

	delete _text;
}

bool UISpriteText::Process() { return true; }

bool UISpriteText::Draw() {
	_text->Draw();
	return true;
}