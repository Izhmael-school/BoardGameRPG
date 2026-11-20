#include "UIWindow.h"
#include "Sprite/MySprite.h"
#include "Dxlib.h"

UIWindow::UIWindow(int _backColor, int _frameColor, int _x, int _y, int _w, int _h, bool _isCenter,int _frameSize, std::string _uiName)
	:UIObject(_uiName)
	, text()
	, textStyle()
	, backColor(_backColor)
	, frameColor(_frameColor)
	, frameSize(_frameSize)
	, w(_w)
	, h(_h)
	, dx(0)
	, dy(0)
	, isFit(false) 
	,isCenter(_isCenter)
{
	position = Vector2(_x, _y);
}

UIWindow::UIWindow(std::string _text, UITextStyle _textStyle, int _backColor, int _frameColor, int _x, int _y, int _frameSize, std::string _uiName)
	:UIObject(_uiName)
	, text(_text)
	, textStyle(_textStyle)
	, backColor(_backColor)
	, frameColor(_frameColor)
	, frameSize(_frameSize)
	, w(0)
	, h(0)
	, dx(0)
	, dy(0)
	, isFit(true) 
	,isCenter(true)
{
	position = Vector2(_x, _y);
}

void UIWindow::OnInit() {
	if (!isCenter) return;

	// 中心に合わせる
	dx = position.x;
	dy = position.y;
	position = MySprite::BoxCenterPos(dx, dy, w, h);

	if (!isFit) return;

	// 文字数、文字サイズに応じてウィンドウのサイズを作る
	// 全角のため割る2で対応
	w = static_cast<int>(text.size() / 2) * textStyle.fontSize;
	h = textStyle.fontSize;
	// 文字も中心に合わせる
	textStyle.textDrawPosition = TextDrawPosition_Center;
	textUI = std::make_unique<UIText>(text, textStyle, Vector2(position.x + w / 2, position.y + h / 2));
	textUI->Init();
}

void UIWindow::OnUpdate(float _t, UIInput& _input) {
}

void UIWindow::OnRender() {
	Vector2 wPos = GetWorldPosition();

	DrawFillBox(wPos.x, wPos.y, wPos.x + w, wPos.y + h, backColor);

	if (textUI)
		textUI->Render();

	float frameHalf = (frameSize / 2);
	DrawLineBox(wPos.x - frameHalf - 1, wPos.y - frameHalf - 1, wPos.x + w + frameHalf, wPos.y + h + frameHalf, frameColor, frameSize);
	DrawCircle(wPos.x - frameHalf, wPos.y - frameHalf, 16, frameColor, true);
}

void UIWindow::OnEnd() {
}

void UIWindow::SetText(const std::string& _text) {
	if (!textUI) return;

	textUI->SetText(_text);
	// 中心に合わせる
	position = MySprite::BoxCenterPos(dx, dy, w, h);
	textUI->SetPosition(position);
	const TCHAR* drawText = text.c_str();
	// 文字数、文字サイズに応じてウィンドウのサイズを作る
	int lineCount;
	GetDrawStringSizeToHandle(&w, &h, &lineCount, drawText, (int)_tcslen(drawText), textUI->GetFontHandle());
}
