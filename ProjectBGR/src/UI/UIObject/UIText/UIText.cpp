#include "UIText.h"
#include "DxLib.h"
#include "Definition/CommonModule/String/MyString.h"
#include "Manager/Font/FontManager.h"

UIText::UIText()
	:text("テストテキスト")
	, style(UITextStyle())
	, fontHandle(-1)
	, isSelect(false)
	, canSelect(true) {
	SetPosition(0, 0);
}

UIText::UIText(const std::string _text, UITextStyle _style, const Vector2& _pos)
	:text(_text)
	, style(_style)
	, fontHandle(-1)
	, isSelect(false)
	, canSelect(true) {
	SetPosition(_pos);
}


constexpr int D_FONT_SIZE = 18;
void UIText::OnInit() {
	Build();
	// テキストスタイルからテキストの位置をもらう
	TextDrawPosition drawPos = style.textDrawPosition;
	int x = position.x, y = position.y;
	switch (drawPos) {
	case TextDrawPosition_Left:
		MyString::StringCenterPos(text, fontHandle, &x, &y,D_FONT_SIZE / style.fontSize, D_FONT_SIZE / style.fontSize);
		position.y = y;
		// 左端揃え
		break;
	case TextDrawPosition_Center:
		// 中央揃え
		MyString::StringCenterPos(text, fontHandle, &x, &y, D_FONT_SIZE / style.fontSize, D_FONT_SIZE / style.fontSize);
		position = Vector2(x, y);
		break;
	case TextDrawPosition_Right:
		// 右端揃え
		int sx;
		MyString::StringCenterPos(text, fontHandle, &sx, &y, D_FONT_SIZE / style.fontSize, D_FONT_SIZE / style.fontSize);
		x = MyString::StringRightPos(text, fontHandle, x, D_FONT_SIZE / style.fontSize);
		position = Vector2(x, y);
		break;
	}
}

void UIText::OnUpdate(float _t) {
}

void UIText::OnRender() {
	unsigned int color = -1;
	if (!canSelect)
		color = style.cantSelectColor;
	else if (isSelect)
		color = style.selectedColor;
	else
		color = style.normalColor;

	Vector2 pos = GetWorldPosition();

	DrawStringToHandle(pos.x, pos.y, text.c_str(), color, fontHandle, style.outlineColor);
}

void UIText::OnEnd() {
}

void UIText::Build() {
	fontHandle = FontManager::GetInstance().GetFontHandle(
		style.fontName,
		style.fontSize,
		style.fontThickness,
		style.fontType,
		style.edgeSize,
		style.italic
	);
}
