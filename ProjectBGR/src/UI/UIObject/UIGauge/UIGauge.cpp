#include "UIGauge.h"
#include "DxLib.h"
#include "Sprite/MySprite.h"

UIGauge::UIGauge(Vector2 _pos, int _w, int _h, float _value, DecreasingDirection _dd, std::string _name)
	:UIObject(_name)
	,w(_w)
	,h(_h)
	,dd(_dd)
{
	position = _pos;
	CalcCenter();
}

void UIGauge::OnUpdate(float _t, UIInput& _input) {
	CalcCenter();
	CalcDecreasingPos();
}

void UIGauge::OnRender() {

	// 下地
	DrawFillBox(cx, cy, cx + w,  cy + h, 0x000000);
	// 実際に減る部分
	DrawFillBox(l, t, r + l, t + b, 0x00ff00);
}

void UIGauge::CalcCenter() {
	Vector2 wPos = GetWorldPosition();

	Vector2 cv = MySprite::BoxCenterPos(wPos.x, wPos.y, w, h);
	cx = cv.x;
	cy = cv.y;
}

int UIGauge::CalcDecreasingPos() {

	l = cx;
	r = w;
	t = cy;
	b = h;

	switch (dd) {
	case DD_Left:
	{
		int cl = (l - r) + (r * value);
		r = r - (l - cl);
		l += l - cl;
	}
		break;
	case DD_Right:
		r = r * value;
		break;
	case DD_Top:
	{
		int ct = (t - b) + (b * value);
		b = b - (t - ct);
		t += t - ct;
	}
		break;
	case DD_Bottom:
		b = b * value;
		break;
	}
	return 0;
}
