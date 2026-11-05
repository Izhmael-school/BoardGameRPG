#include "UISprite.h"
#include "DxLib.h"
#include "Definition/CommonModule/Math/MyMath.h"
#include "Sprite/MySprite.h"

UISprite::UISprite(int _handle, Vector2 _pos, float _ex, float _ey, float _rot, std::string _name)
	:UIObject(_name)
	, graphHandle(_handle)
	, ex(_ex)
	, ey(_ey)
	, cx(-1)
	, cy(-1) {
	position = _pos;

	angle = MyMath::Deg2Rad(_rot);
	Vector2 c = MySprite::GetSpriteSize(graphHandle);
	cx = c.x / 2;
	cy = c.y / 2;
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
