#pragma once
#include"UI.h"
#include<vector>
#include"Dxlib.h"

class UIVision :public UI {
public:
	UIVision(class ObjectServer* server, int drawOrder = 100);
	~UIVision();

	const char* GetName()override { return "Vision"; }

	bool Process()override;
	bool Draw()override;

private:

	int _handle;								//画像ハンドル

	std::vector<unsigned short>_versNums;		//頂点番号
	std::vector<VERTEX3D>_vers;					//頂点情報

	class ObjectServer* _server;				//オブジェクトサーバー

};