#include "MainGameCanvas.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIText/UITextDialog/UITextDialog.h"
#include "UI/UIObject/UIButton/UIButton.h"
#include "UI/UIObject/UIGauge/UIGauge.h"
#include "UI/UIObject/UIWindow/UIWindow.h"
#include "UI/UIObject/UIBillboard/UIBillboard.h"
#include "UI/Input/UIInput.h"
#include "String/MyString.h"

void MainGameCanvas::Init() {
	LoadCanvasForJson("res/ExternalFile/UI/Text.json");

	auto dialog = Find<UITextDialog>("dialog");
	if (dialog)
		dialog->AddText("テストテキストだお", []() {return false; });
	auto billboard = Find<UIBillboard>("billboard");
	if (billboard)
		billboard->SetDrawRange(0, 3);
}

void MainGameCanvas::Update(float _t, UIInput& _input) {
	static float value = 1.0f;

	if (_input.IsKeyDown(KEY_LEFT))
		value -= 0.05f;

	if (_input.IsKeyDown(KEY_RIGHT))
		value += 0.05f;



	UICanvasBase::Update(_t, _input);
}
