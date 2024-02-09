#pragma once
#include"appframe.h"

class ModeLightsOut :public ModeBase {
public:
	ModeLightsOut(class ModeGame* game);
	bool Initialize()override;
	bool Terminate()override;
	bool Process()override;
	bool Render()override;
protected:
	//画面に貼るエフェクト
	int _cg;
	//上記のエフェクトの透明度
	int _alpha;

	class Animation2D {
	public:
		Animation2D(std::vector<int>Cgs,int AnimSpeed,int x ,int y,int w,int h) 
			:_cgs(Cgs)
			,_animSpeed(AnimSpeed)
			,_animCnt(0)
			,_frameCnt(0)
			,_x(x)
			,_y(y)
			,_w(w)
			,_h(h)
		{
			
		}
		virtual ~Animation2D(){}
		bool Draw() {
			DrawModiGraph(
				_x, _y,
				_x + _w, _y,
				_x + _w, _y + _h,
				_x, _y + _h,
				_cgs[_frameCnt / _animSpeed],
				TRUE
			);

			_frameCnt++;
			if (_frameCnt / _animSpeed == _cgs.size()) {
				_frameCnt = 0;
			}
			return true;
		}
	protected:
		std::vector<int>_cgs;
		int _animSpeed;
		int _animCnt;
		int _frameCnt;
		int _x, _y, _w, _h;
	};

	//画面に貼るエフェクトの上にかけるノイズのアニメーション
	Animation2D* _noiseAnim;

	//処理時間
	int _frameCnt;

	int _oldEnemyNum;

	class ModeGame* _game;
};