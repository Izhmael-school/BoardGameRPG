#include "UIButton.h"
#include "DxLib.h"
#include "Manager/Font/FontManager.h"
#include "Definition/CommonModule/String/Mystring.h"
#include "UI/Input/UIInput.h"

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

UIButton::UIButton(Vector2 _pos, UIButtonStyle _style, const std::string& _text, const std::string& _selectText, const std::string& _cantSelectText)
	:style(_style)
	, text(_text)
	, selectText(_selectText)
	, cantSelectText(_cantSelectText)
	, cx(_pos.x)
	, cy(_pos.y)
	, canSelect(true) {
	position = _pos;
}

void UIButton::OnInit() {
	UITextStyle s = style.textStyle;
	fontHandle = FontManager::GetInstance().GetFontHandle(s.fontName, s.fontSize, s.fontThickness, s.fontType, s.edgeSize, s.italic);
	GetCenterPos(style.graphHandle, position, 1, 1);
	RePositionText(text);
}

void UIButton::OnUpdate(float _t, UIInput& _input) {
	if (!canSelect) return;

	// マウスがボタンの範囲内にいるか確認
	isSelect = OnMouse(_input.mousePos);

	if (!isSelect) return;

	// マウスが押されたら
	if (_input.IsMouseDown(MOUSE_LEFT))
		OnClick();
}

void UIButton::OnRender() {
#if _DEBUG
	Vector2 b1 = Vector2(position.x - style.cx, position.y - style.cy);
	Vector2 b2 = Vector2(position.x + style.cx, position.y + style.cy);
	DrawLineBox(b1.x, b1.y, b2.x, b2.y, 0x00ff00);
#endif


	std::string drawText = text;
	unsigned int color = style.textStyle.normalColor;
	int drawHandle = style.graphHandle;

	// 選択されてるとき
	if (isSelect) {
		if (!selectText.empty()) {
			drawText = selectText;

		}

		int select = style.selectGraphHandle;
		if (select != -1)
			drawHandle = select;

		color = style.textStyle.selectedColor;
	}

	// 選択できないとき
	if (!canSelect) {
		if (!cantSelectText.empty())
			drawText = cantSelectText;

		int cantSelect = style.cantSelectGraphHandle;
		if (cantSelect != -1)
			drawHandle = cantSelect;

		color = style.textStyle.cantSelectColor;
	}

	if (drawHandle != -1)
		DrawGraph(position.x, position.y, drawHandle, TRUE);
	else
		DrawFillBox(position.x - style.cx, position.y - style.cy, position.x + style.cx, position.y + style.cy, 0xffffff);

	if (!drawText.empty()) {
		RePositionText(drawText);
		DrawStringToHandle(cx, cy, drawText.c_str(), color, fontHandle, style.textStyle.outlineColor);
	}

}

void UIButton::OnEnd() {
}

bool UIButton::OnMouse(Vector2 _mousePos) {
	Vector2 b1 = Vector2(position.x - style.cx, position.y - style.cy);
	Vector2 b2 = Vector2(position.x + style.cx, position.y + style.cy);

	if (b1.x <= _mousePos.x && b1.y <= _mousePos.y && b2.x > _mousePos.x && b2.y > _mousePos.y)
		return true;

	return false;
}

void UIButton::OnClick() {
	if (onClick)
		onClick();
}

void UIButton::RePositionText(const std::string& _drawText) {
	cx = position.x;
	cy = position.y;
	int sx = -1;

	TextDrawPosition drawPos = style.textStyle.textDrawPosition;
	switch (drawPos) {
	case TextDrawPosition_Left:
		MyString::StringCenterPos(_drawText, fontHandle, &sx, &cy);
		// 左端揃え
		break;
	case TextDrawPosition_Center:
		// 中央揃え
		MyString::StringCenterPos(_drawText, fontHandle, &cx, &cy);
		break;
	case TextDrawPosition_Right:
		// 右端揃え
		MyString::StringCenterPos(_drawText, fontHandle, &sx, &cy);
		cx = MyString::StringRightPos(_drawText, fontHandle, cx);
		break;
	}
}
