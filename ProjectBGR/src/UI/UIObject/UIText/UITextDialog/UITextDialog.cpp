#include "UITextDialog.h"
#include "UI/Input/UIInput.h"
#include "DxLib.h"
#include "String/MyString.h"

UITextDialog::UITextDialog(UITextStyle _style, const Vector2& _pos, int backImage, std::string _uiName)
	:UIText("", _style, _pos, _uiName)
	, nextCharTime(0.05f)
	, currentTime(nextCharTime)
	, currentCharIndex(0)
	, drawString("")
	, isAllDraw(false) {
}

void UITextDialog::OnUpdate(float _t, UIInput& _input) {
	if (waitStrings.empty() && text.empty()) {
		SetActive(false);
		return;
	}

	SetActive(true);

	currentTime += _t;

	if (waitStrings.empty()) return;

	if (currentTime >= nextCharTime && !isAllDraw) {
		currentTime = 0;
		std::string drawText = waitStrings.front();
		std::string currentText = MyString::GetUTF8Char(drawText, currentCharIndex);
		// 一文字ずつ描画
		drawString = MyString::MergeString(drawString, currentText);
		if (drawText.size() - 1 <= currentCharIndex)
			isAllDraw = true;
	}

	// 条件を満たしたら次の文字列に進む
	if (nextTextFlags.front()()) {
		DeleteFrontText();
		return;
	}

	if (!isAllDraw && _input.IsKeyDown(KEY_RETURN)) {
		isAllDraw = true;
		currentTime = nextCharTime;
		drawString = waitStrings.front();
	}
}

void UITextDialog::OnRender() {
	DrawFillBox(position.x, position.y, 1920 - position.x, position.y + 320, 0x000000);

	unsigned int color = -1;
	if (!canSelect)
		color = style.cantSelectColor;
	else if (isSelect)
		color = style.selectedColor;
	else
		color = style.normalColor;

	Vector2 pos = GetWorldPosition();

	if (text.empty())
		DrawStringToHandle(pos.x + 10, pos.y + 20, drawString.c_str(), color, fontHandle, style.outlineColor);
	else {
		DrawStringToHandle(pos.x + 10, pos.y + 20, text.c_str(), color, fontHandle, style.outlineColor);
		text.clear();
	}
}

void UITextDialog::DeleteFrontText() {
	if (!waitStrings.empty())
		waitStrings.erase(waitStrings.begin());

	if (!nextTextFlags.empty())
		nextTextFlags.erase(nextTextFlags.begin());

	isAllDraw = false;
	currentCharIndex = 0;
	currentTime = nextCharTime;
	drawString.clear();
}
