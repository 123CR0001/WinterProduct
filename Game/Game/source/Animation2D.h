#pragma once
#include<vector>
class Animation2D {
public:
	Animation2D(std::vector<int>Cgs, int AnimSpeed, int x, int y, int w, int h)
		:_cgs(Cgs)
		, _animSpeed(AnimSpeed)
		, _frameCnt(0)
		, _x(x)
		, _y(y)
		, _w(w)
		, _h(h)
	{

	}
	virtual ~Animation2D() {}
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
		if(_frameCnt / _animSpeed == _cgs.size()) {
			_frameCnt = 0;
		}
		return true;
	}
protected:
	std::vector<int>_cgs;
	int _animSpeed;
	int _frameCnt;
	int _x, _y, _w, _h;
};