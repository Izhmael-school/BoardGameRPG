#include "MainGameCanvas.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIButton/UIButton.h"
#include "UI/UIObject/UIGauge/UIGauge.h"
#include "UI/UIObject/UIWindow/UIWindow.h"
#include "UI/UIObject/UIBillboard/UIBillboard.h"
#include "UI/Input/UIInput.h"
#include "String/MyString.h"

void MainGameCanvas::Init() {
	auto bill = Instantiate<UIBillboard>(Vector2(400, 200),1, 30, 30, true);
	auto text = UniqueInstantiate<UIText>("a", UITextStyle(), VZero_2);
	bill->AddChild(std::move(text));
	text = UniqueInstantiate<UIText>("a", UITextStyle(), VZero_2);
	bill->AddChild(std::move(text));
	text = UniqueInstantiate<UIText>("aaa", UITextStyle(), VZero_2);
	bill->AddChild(std::move(text));
	text = UniqueInstantiate<UIText>("aa", UITextStyle(), VZero_2);
	bill->AddChild(std::move(text));
	text = UniqueInstantiate<UIText>("aaaaaa", UITextStyle(), VZero_2);
	bill->AddChild(std::move(text));
	text = UniqueInstantiate<UIText>("aaa", UITextStyle(), VZero_2);
	bill->AddChild(std::move(text));

	bill->SetSelect(2);
	bill->SetDrawRange(2, 4);
}

void MainGameCanvas::Update(float _t, UIInput& _input) {
	static float value = 1.0f;

	if (_input.IsKeyDown(KEY_LEFT))
		value -= 0.05f;

	if (_input.IsKeyDown(KEY_RIGHT))
		value += 0.05f;



	UICanvasBase::Update(_t, _input);
}
