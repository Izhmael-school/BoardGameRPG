#include "UISprite.h"
#include "DxLib.h"
#include "Definition/CommonModule/Math/MyMath.h"

namespace {
	Vector2 GetCenterPos(int _handle, Vector2 _pos, int _ex, int _ey) {
		int w = -1, h = -1;
		GetGraphSize(_handle, &w, &h);

		Vector2 pos;

		pos.x = _pos.x - ((w * _ex) / 2);
		pos.x = _pos.y - ((h * _ey) / 2);

		return pos;
	}
}

UISprite::UISprite(int _handle, Vector2 _pos, float _ex, float _ey, float _rot)
	:graphHandle(_handle)
	, ex(_ex)
	, ey(_ey)
	, cx(_pos.x)
	, cy(_pos.y) {
	//position = GetCenterPos(_handle, _pos, _ex, _ey);
	position = _pos;
	angle = MyMath::Deg2Rad(_rot);
}

void UISprite::OnInit() {
}

void UISprite::OnUpdate(float _t) {
}

void UISprite::OnRender() {
	Vector2 wPos = GetWorldPosition();

	if (graphHandle == -1) {
		DrawBox(wPos.x, wPos.y, wPos.x + (1 * ex), wPos.y + (1 * ey), 0xffffff, TRUE);
		return;
	}

	DrawRotaGraph3F(wPos.x, wPos.y,cx,cy,ex,ey,angle, graphHandle, TRUE);
}

void UISprite::OnEnd() {
}
