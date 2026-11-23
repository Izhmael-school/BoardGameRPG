#include "UIRoulette.h"
#include "DxLib.h"
#include "Sprite/MySprite.h"
#include "Definition/CommonModule/Math/MyMath.h"

UIRoulette::UIRoulette(int _rouletteHandle, int _arrowHandle, Vector2 _pos, float _ex, float _ey, std::string _name)
	:UISprite(_rouletteHandle, _pos, _ex, _ey, 0, _name)
	, arrowHandle(_arrowHandle)
	, aex(_ex)
	, aey(_ey)
	, aAngle(0)
	, acx(0)
	, acy(0) {

}

UIRoulette::UIRoulette(std::string _name)
	:UISprite(-1, VZero_2, 1.0f, 1.0f, 0, _name)
	, arrowHandle(-1)
	, aex(1.0f)
	, aey(1.0f)
	, aAngle(0)
	, acx(0)
	, acy(0) {

}

void UIRoulette::OnInit() {
	Vector2 ac = MySprite::GetSpriteSize(arrowHandle);
	acx = ac.x / 2;
	acy = ac.y / 2;
}

void UIRoulette::OnRender() {
	Vector2 wPos = GetWorldPosition();

	// ルーレット自体はこっちで描画
	UISprite::OnRender();
	// 針の描画
	DrawRotaGraph3F(wPos.x - 1, wPos.y - 1, acx, acy, aex, aey, aAngle, arrowHandle, TRUE);
}

void UIRoulette::SetArrowAngle(float _angle) {
	aAngle = MyMath::Deg2Rad(_angle);
}
