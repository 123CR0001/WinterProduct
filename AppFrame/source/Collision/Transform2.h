#pragma once
#include"Vector2.h"

class Transform2 {
public:

	Vector2 pos;
	float rad;
	float zoom;

	Transform2()
		:rad(0.f)
		,zoom(1.f)
	{}
	Transform2(const Vector2& pos)
		:pos(pos)
		, rad(0.f)
		, zoom(1.f)
	{}
	Transform2(float rad,float zoom, const Vector2& pos)
		:pos(pos)
		,rad(rad)
		,zoom(zoom)
	{}
	Transform2(const Transform2& transform) {
		*this = transform;
	}

	void operator = (const Transform2& right) {
		pos = right.pos;
		rad = right.rad;
		zoom = right.zoom;
	}

};