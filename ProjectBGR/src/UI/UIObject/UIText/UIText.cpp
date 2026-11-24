#include "UIText.h"
#include "DxLib.h"
#include "Definition/CommonModule/String/MyString.h"
#include "Manager/Font/FontManager.h"
#include "Library/Vector/ConversionVECTOR.h"
#include <string.h> 

UIText::UIText()
	:text("テストテキスト")
	, style(UITextStyle())
	, fontHandle(-1)
	, isSelect(false)
	, canSelect(true) {
	SetPosition(0, 0);
}

UIText::UIText(std::string _name) 
	:UIObject(_name)
	,text("テストテキスト")
	, style(UITextStyle())
	, fontHandle(-1)
	, isSelect(false)
	, canSelect(true)
{
}

UIText::UIText(const std::string _text, UITextStyle _style, const Vector2& _pos, const std::string& _name)
	:UIObject(_name)
	,text(_text)
	, style(_style)
	, fontHandle(-1)
	, isSelect(false)
	, canSelect(true)
	,basePos(_pos)
{
	SetPosition(_pos);
}


constexpr int D_FONT_SIZE = 18;
void UIText::OnInit() {
	basePos = position;
	Build();
	Reposition();
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

	DrawExtendStringToHandle(pos.x, pos.y,style.exRateX,style.exRateY, text.c_str(), color, fontHandle, style.outlineColor);
}

void UIText::OnEnd() {
}

void UIText::Reposition() {
	// テキストスタイルからテキストの位置をもらう
	TextDrawPosition drawPos = style.textDrawPosition;
	int x = basePos.x, y = basePos.y;
	float exRateX = style.exRateX;
	float exRateY = style.exRateY;
	switch (drawPos) {
	case TextDrawPosition_Left:
		MyString::StringCenterPos(text, fontHandle, &x, &y, exRateX, exRateY);
		position.y = y;
		// 左端揃え
		break;
	case TextDrawPosition_Center:
		// 中央揃え
		MyString::StringCenterPos(text, fontHandle, &x, &y, exRateX, exRateY);
		position = Vector2(x, y);
		break;
	case TextDrawPosition_Right:
		// 右端揃え
		int sx;
		MyString::StringCenterPos(text, fontHandle, &sx, &y, exRateX, exRateY);
		x = MyString::StringRightPos(text, fontHandle, x, exRateX);
		position = Vector2(x, y);
		break;
	}
}

void UIText::RepositionX() {
	// テキストスタイルからテキストの位置をもらう
	TextDrawPosition drawPos = style.textDrawPosition;
	int x = position.x, y = position.y;
	float exRateX = style.exRateX;
	float exRateY = style.exRateY;
	int sy;
	switch (drawPos) {
	case TextDrawPosition_Center:
		// 中央揃え
		MyString::StringCenterPos(text, fontHandle, &x, &sy, exRateX, exRateY);
		position = Vector2(x, y);
		break;
	case TextDrawPosition_Right:
		// 右端揃え
		x = MyString::StringRightPos(text, fontHandle, x, exRateX);
		position = Vector2(x,y);
		break;
	}
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

Vector2 UIText::GetEndPosition() const {
	int sx = 0, sy = 0, line = 0;

	int len = strlen(text.c_str());

	GetDrawStringSizeToHandle(&sx, &sy, &line, text.c_str(), len, fontHandle);

	return Vector2(sx + position.x,sy + position.y);
}
