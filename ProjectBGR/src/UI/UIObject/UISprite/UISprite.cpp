#include "UISprite.h"
#include "DxLib.h"
#include "Sprite/MySprite.h"
#include "Definition/CommonModule/Math/MyMath.h"

UISprite::UISprite(int _handle, Vector2 _pos, float _ex, float _ey, float _rot, std::string _name)
	:UIObject(_name)
	, graphHandle(_handle)
	, ex(_ex)
	, ey(_ey)
	, cx(-1)
	, cy(-1) {
	position = _pos;

	SetAngle(_rot);
	CalcCenter();
}

UISprite::UISprite(std::string _name)
	:UIObject(_name)
	, graphHandle(-1)
	, ex(1.0f)
	, ey(1.0f)
	, cx(-1)
	, cy(-1) {
	position = VZero_2;

	SetAngle(0);
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

	DrawRotaGraph3F(wPos.x, wPos.y, cx, cy, ex, ey, angle, graphHandle, TRUE);

}

void UISprite::OnEnd() {
}

void UISprite::CalcCenter() {
	Vector2 c = MySprite::GetSpriteSize(graphHandle);
	cx = c.x / 2;
	cy = c.y / 2;
}

void UISprite::SetAngle(float _rot) { angle = MyMath::Deg2Rad(_rot); }
