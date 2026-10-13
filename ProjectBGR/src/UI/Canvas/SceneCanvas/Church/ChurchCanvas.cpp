#include "ChurchCanvas.h"
#include "UI/UIObject/UIText/UITextDialog/UITextDiaLog.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIButton/UIButton.h"
#include "UI/UIObject/UISprite/UISprite.h"
#include "UI/Input/UIInput.h"

void ChurchCanvas::Init() {
	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	dialog = Instantiate<UITextDialog>(explanation, Vector2(250, 700));
	dialog->SetActive(false);
	// Selectの設定
	select = Instantiate<UIObject>();
	select->SetPosition(Vector2(0, 0));
	UITextStyle textStyle;
	textStyle.fontSize = 50;
	textStyle.textDrawPosition = TextDrawPosition_Center;
	textStyle.normalColor = 0x000000;

	int posX = 384;
	int posY = 800;

	auto recoveryButton = UniqueInstantiate<UIText>("祈り", textStyle, Vector2(posX, posY));
	select->AddChild(std::move(recoveryButton));
	auto rpButton = UniqueInstantiate<UIText>("転移", textStyle, Vector2(posX * 2, posY));
	select->AddChild(std::move(rpButton));
	auto warpButton = UniqueInstantiate<UIText>("回復", textStyle, Vector2(posX * 3, posY));
	select->AddChild(std::move(warpButton));
	auto leaveButton = UniqueInstantiate<UIText>("帰る", textStyle, Vector2(posX * 4, posY));
	select->AddChild(std::move(leaveButton));
}

void ChurchCanvas::Update(float _t, UIInput& _input) {
	// 状態が変わったら前の状態のUIを閉じる
	if (churchState != prevChurchState) {
		switch (prevChurchState) {
		case ChurchState_SetRP:
			CloseSetRPUI();
			break;
		case ChurchState_Warp:
			CloseWarpUI();
			break;
		case ChurchState_Recovery:
			CloseRecoveryUI();
			break;
		}

		prevChurchState = churchState;
	}

	switch (churchState) {
	case ChurchState_Selection:
	{
		select->SetActive(true);
		auto children = select->GetChildren();
		for (size_t i = 0, max = children.size(); i < max; i++) {
			auto text = static_cast<UIText*>(children[i]);

			if (!text) continue;

			if (selectStateIndex != i) {
				text->SetSelect(false);
				continue;
			}

			text->SetSelect(true);
		}
	}
	break;
	case ChurchState_SetRP:
		// UIを作ってなければ作る
		if (!setRP)
			OpenSetRPUI();

		UpdateSetRPUI();
		break;
	case ChurchState_Warp:
		// UIを作ってなければ作る
		if (!warp)
			OpenWarpUI();

		UpdateWarpUI();
		break;
	case ChurchState_Recovery:
		// UIを作ってなければ作る
		if (!recovery)
			OpenRecoveryUI();

		UpdateRecoveryUI();
		break;
	}

	UICanvasBase::Update(_t, _input);
}

void ChurchCanvas::SetNeedInfo(ChurchUINeedInfo _info) {
	prevChurchState = churchState;
	churchState = _info.selection;
	selectStateIndex = _info.selectionSectionIndex;
	selectCommandIndex = _info.commandSelectIndex;
	reason = _info.dontReceiveServiceReason;
}

void ChurchCanvas::OpenRecoveryUI() {
	// 既に開いていたら帰る
	if (recovery) return;

	select->SetActive(false);

	recovery = Instantiate<UIObject>();
	UITextStyle style;
	auto textYes = UniqueInstantiate<UIText>("はい", style, Vector2(300, 300));
	auto textNo = UniqueInstantiate<UIText>("いいえ", style, Vector2(300, 320));
	recovery->AddChild(std::move(textYes));
	recovery->AddChild(std::move(textNo));
}

void ChurchCanvas::CloseRecoveryUI() {
	// 開いてなければ帰る
	if (!recovery) return;

	select->SetActive(true);

	dialog->SetText("");

	DeleteUI(recovery);
}

void ChurchCanvas::UpdateRecoveryUI() {
	// 開いてなければ帰る
	if (!recovery) return;

	std::vector<UIObject*> children = recovery->GetChildren();

	for (int i = 0, max = children.size(); i < max; i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		text->SetSelect(selectCommandIndex == i);
	}

	std::string drawText = "";

	switch (reason) {
	case DontReceiveServiceReason_Invalid:
		drawText = "体力を回復しますか？";
		break;
	case DontReceiveServiceReason_ReceiveService:
		drawText = "あなたに幸多からんことを。";
		break;
	case DontReceiveServiceReason_NoMoney:
		drawText = "お金が足りません。";
		break;
	case DontReceiveServiceReason_NotNeed:
		drawText = "今は必要ないでしょう。";
		break;
	}

	dialog->SetText(drawText);
}

void ChurchCanvas::OpenWarpUI() {
	// 既に開いていたら帰る
	if (warp) return;

	select->SetActive(false);

	dialog->SetText("どこに転移しますか？");

	warp = Instantiate<UIObject>();

	auto churchs = getCanWarpChurchsFunc();

	UITextStyle style;
	int index = 0;
	for (auto& church : churchs) {
		auto text = UniqueInstantiate<UIText>(church.churchName, style, Vector2(300, 300 + 20 * index));
		warp->AddChild(std::move(text));
		index++;
	}
	auto text = UniqueInstantiate<UIText>("やめる", style, Vector2(300, 300 + 20 * index));
	warp->AddChild(std::move(text));
}

void ChurchCanvas::CloseWarpUI() {
	// 開いてなければ帰る
	if (!warp) return;

	select->SetActive(true);

	dialog->SetText("");

	DeleteUI(warp);
}

void ChurchCanvas::UpdateWarpUI() {
	// 開いてなければ帰る
	if (!warp) return;

	std::vector<UIObject*> children = warp->GetChildren();

	for (int i = 0, max = children.size(); i < max; i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		text->SetSelect(selectCommandIndex == i);
	}
}

void ChurchCanvas::OpenSetRPUI() {
	// 既に開いていたら帰る
	if (setRP) return;

	select->SetActive(false);

	setRP = Instantiate<UIObject>();
	UITextStyle style;
	auto textYes = UniqueInstantiate<UIText>("はい", style, Vector2(300, 300));
	auto textNo = UniqueInstantiate<UIText>("いいえ", style, Vector2(300, 320));
	setRP->AddChild(std::move(textYes));
	setRP->AddChild(std::move(textNo));
}

void ChurchCanvas::CloseSetRPUI() {
	// 開いてなければ帰る
	if (!setRP) return;

	select->SetActive(true);

	dialog->SetText("");

	DeleteUI(setRP);
}

void ChurchCanvas::UpdateSetRPUI() {
	// 開いてなければ帰る
	if (!setRP) return;

	std::vector<UIObject*> children = setRP->GetChildren();

	for (int i = 0, max = children.size(); i < max; i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		text->SetSelect(selectCommandIndex == i);
	}

	std::string drawText = "";

	switch (reason) {
	case DontReceiveServiceReason_Invalid:
		drawText = "祈りますか？";
		break;
	case DontReceiveServiceReason_ReceiveService:
		drawText = "あなたに幸多からんことを。";
		break;
	}

	dialog->SetText(drawText);
}