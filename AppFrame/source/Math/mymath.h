#pragma once
	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include <cmath>
#include <iostream>
// 円周率
#define	PI				(3.141592653589793f)

// radian→degree
float DegToRad(float deg);

// degree→radian
float RadToDeg(float rad);


// 当たり判定用。2つのboxが当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitBox(
	int x1, int y1, int w1, int h1,		// ひとつめのbox 左上(x,y), 大きさw,h
	int x2, int y2, int w2, int h2		// ふたつめのbox 左上(x,y), 大きさw,h
);


// 当たり判定用。2つの円が当たったかを判定
// 当たっていたら1, 当たっていなかったら0を返す
int IsHitCircle(
	int x1, int y1, int r1,		// ひとつめのcircle 中心(x,y), 半径r
	int x2, int y2, int r2		// ふたつめのcircle 中心(x,y), 半径r
);

bool IsHitPointRect(float point_x, float point_y, float rect_x, float rect_y, float rect_w, float rect_h);


struct MYRECT {
	float x, y, w, h;
};

template <typename T>
void swap(T& x, T& y)
{
	T temp = std::move(x);
	x = std::move(y);
	y = std::move(temp);
}

// 様々なイージング
// https://game-ui.net/?p=835 を参考に
template <typename T>
T EasingLinear(T start, T end, T rate) {
	return (end - start) * rate + start;
};
// イージング
float EasingLinear(float cnt, float start, float end, float frames);
float EasingInQuad(float cnt, float start, float end, float frames);
float EasingOutQuad(float cnt, float start, float end, float frames);
float EasingInOutQuad(float cnt, float start, float end, float frames);
float EasingInCubic(float cnt, float start, float end, float frames);
float EasingOutCubic(float cnt, float start, float end, float frames);
float EasingInOutCubic(float cnt, float start, float end, float frames);
float EasingInQuart(float cnt, float start, float end, float frames);
float EasingOutQuart(float cnt, float start, float end, float frames);
float EasingInOutQuart(float cnt, float start, float end, float frames);
float EasingInQuint(float cnt, float start, float end, float frames);
float EasingOutQuint(float cnt, float start, float end, float frames);
float EasingInOutQuint(float cnt, float start, float end, float frames);
float EasingInSine(float cnt, float start, float end, float frames);
float EasingOutSine(float cnt, float start, float end, float frames);
float EasingInOutSine(float cnt, float start, float end, float frames);
float EasingInExpo(float cnt, float start, float end, float frames);
float EasingOutExpo(float cnt, float start, float end, float frames);
float EasingInOutExpo(float cnt, float start, float end, float frames);
float EasingInCirc(float cnt, float start, float end, float frames);
float EasingOutCirc(float cnt, float start, float end, float frames);
float EasingInOutCirc(float cnt, float start, float end, float frames);


// 範囲内を返す
float Clamp(float min_n, float max_n, float n);

