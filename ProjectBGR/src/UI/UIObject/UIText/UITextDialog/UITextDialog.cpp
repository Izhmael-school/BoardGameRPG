#include "UITextDialog.h"
#include "UI/Input/UIInput.h"
#include "DxLib.h"

UITextDialog::UITextDialog(UITextStyle _style, const Vector2& _pos, int backImage)
	:UIText("", _style, _pos)
	, nextCharTime(0.1f)
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
		std::string text = waitStrings.front();
		// 一文字ずつ描画
		drawString.push_back(text[currentCharIndex]);
		currentCharIndex++;
		// すべて描画し終わった
		if (text.size() - 1 <= currentCharIndex)
			isAllDraw = true;
	}


	if (isAllDraw && _input.IsKeyDown(KEY_RETURN)) {
		isAllDraw = false;
		currentCharIndex = 0;
		currentTime = nextCharTime;
		waitStrings.erase(waitStrings.begin());
		drawString.clear();
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
