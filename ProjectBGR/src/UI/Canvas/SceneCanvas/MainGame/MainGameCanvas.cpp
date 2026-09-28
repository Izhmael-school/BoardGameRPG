#include "MainGameCanvas.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIButton/UIButton.h"

void MainGameCanvas::Init() {
	UITextStyle style;
	Instantiate<UIText>("oh-yes....",style,Vector2(100,400));

	UIButtonStyle bSytle;
	bSytle.cx = 100;
	bSytle.cy = 50;
	UITextStyle btStyle;
	btStyle.textDrawPosition = TextDrawPosition_Center;
	bSytle.textStyle = btStyle;
	UIButton* b = Instantiate<UIButton>(Vector2(100, 600), bSytle, "ボタンのテスト","選択中","選択不可");
	b->SetClickEvent([this, b]() {b->SetCanSelect(false); });
}
