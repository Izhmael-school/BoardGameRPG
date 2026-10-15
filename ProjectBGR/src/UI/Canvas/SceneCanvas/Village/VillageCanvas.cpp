#include "VillageCanvas.h"
#include "UI/UIObject/UIText/UiText.h"
#include "UI/UIObject/UIText/UITextDialog/UITextDialog.h"
#include "UI/UIObject/UIObject.h"

void VillageCanvas::Init() {
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

	auto getTaxButton = UniqueInstantiate<UIText>("回収", textStyle, Vector2(posX, posY));
	select->AddChild(std::move(getTaxButton));
	auto upgradeButton = UniqueInstantiate<UIText>("出資", textStyle, Vector2(posX * 2, posY));
	select->AddChild(std::move(upgradeButton));
	auto acquisitionButton = UniqueInstantiate<UIText>("買収", textStyle, Vector2(posX * 3, posY));
	select->AddChild(std::move(acquisitionButton));
	auto leaveButton = UniqueInstantiate<UIText>("帰る", textStyle, Vector2(posX * 4, posY));
	select->AddChild(std::move(leaveButton));
}

void VillageCanvas::Update(float _t, UIInput& _input) {

	// 状態が変わったら前の状態のUIを閉じる
	if (info.villageState != prevVillageState) {
		switch (prevVillageState) {
		case VillageState_GetTax:
			CloseGetTaxUI();
			break;
		case VillageState_Upgrade:
			CloseUpgradeUI();
			break;
		case VillageState_Acquisition:
			CloseAcquisitionUI();
			break;
		}

		prevVillageState = info.villageState;
	}

	switch (info.villageState) {
	case VillageState_Select:
	{
		select->SetActive(true);
		auto children = select->GetChildren();
		for (size_t i = 0, max = children.size(); i < max; i++) {
			auto text = static_cast<UIText*>(children[i]);

			if (!text) continue;

			if (info.selectionIndex != i) {
				text->SetSelect(false);
				continue;
			}

			text->SetSelect(true);
		}
	}
	break;
	case VillageState_GetTax:
		if (!tax) {
			OpenGetTaxUI();
			return;
		}

		UpdateGetTaxUI();
		break;
	case VillageState_Upgrade:
		if (!upgrade) {
			OpenUpgradeUI();
			return;
		}

		UpdateUpgradeUI();
		break;
	case VillageState_Acquisition:
		if (!acquisition) {
			OpenAcquisitionUI();
			return;
		}

		UpdateAcquisitionUI();
		break;
	}

	UICanvasBase::Update(_t, _input);
}

void VillageCanvas::OpenGetTaxUI() {
	if (tax) return;

	select->SetActive(false);

	tax = Instantiate<UIObject>();
	UITextStyle style;
	auto textYes = UniqueInstantiate<UIText>("受け取る", style, Vector2(300, 300));
	auto textNo = UniqueInstantiate<UIText>("受け取らない", style, Vector2(300, 320));
	tax->AddChild(std::move(textYes));
	tax->AddChild(std::move(textNo));
}

void VillageCanvas::CloseGetTaxUI() {
	// 開いてなければ帰る
	if (!tax) return;

	select->SetActive(true);

	dialog->SetText("");

	DeleteUI(tax);
}

void VillageCanvas::UpdateGetTaxUI() {
	// 開いてなければ帰る
	if (!tax) return;

	std::vector<UIObject*> children = tax->GetChildren();

	for (int i = 0, max = children.size(); i < max; i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		text->SetSelect(info.modeSelectionIndex == i);
	}

	std::string drawText = "";

	switch (info.reason) {
	case CantActReason_DontOwner:
		drawText = "領主じゃないため受け取れません。";
		break;
	case CantActReason_NotEnoughMoney:
		drawText = "お金が貯まっていません。";
		break;
	}

	dialog->SetText(drawText);
}

void VillageCanvas::OpenUpgradeUI() {
	if (upgrade) return;

	select->SetActive(false);

	upgrade = Instantiate<UIObject>();
	UITextStyle style;
	auto textYes = UniqueInstantiate<UIText>("出資する", style, Vector2(300, 300));
	auto textNo = UniqueInstantiate<UIText>("出資しない", style, Vector2(300, 320));
	upgrade->AddChild(std::move(textYes));
	upgrade->AddChild(std::move(textNo));
}

void VillageCanvas::CloseUpgradeUI() {
	// 開いてなければ帰る
	if (!upgrade) return;

	select->SetActive(true);

	dialog->SetText("");

	DeleteUI(upgrade);
}

void VillageCanvas::UpdateUpgradeUI() {
	// 開いてなければ帰る
	if (!upgrade) return;

	std::vector<UIObject*> children = upgrade->GetChildren();

	for (int i = 0, max = children.size(); i < max; i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		text->SetSelect(info.modeSelectionIndex == i);
	}

	std::string drawText = "";

	switch (info.reason) {
	case CantActReason_DontNeed:
		drawText = "最大まで大きくなりました。";
		break;
	case CantActReason_NotEnoughMoney:
		drawText = "お金が足りません。";
		break;
	}

	dialog->SetText(drawText);
}

void VillageCanvas::OpenAcquisitionUI() {
	if (acquisition) return;

	select->SetActive(false);

	acquisition = Instantiate<UIObject>();
	UITextStyle style;
	auto textYes = UniqueInstantiate<UIText>("買収する", style, Vector2(300, 300));
	auto textNo = UniqueInstantiate<UIText>("買収しない", style, Vector2(300, 320));
	acquisition->AddChild(std::move(textYes));
	acquisition->AddChild(std::move(textNo));
}

void VillageCanvas::CloseAcquisitionUI() {
	// 開いてなければ帰る
	if (!acquisition) return;

	select->SetActive(true);

	dialog->SetText("");

	DeleteUI(acquisition);
}

void VillageCanvas::UpdateAcquisitionUI() {
	// 開いてなければ帰る
	if (!acquisition) return;

	std::vector<UIObject*> children = acquisition->GetChildren();

	for (int i = 0, max = children.size(); i < max; i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		text->SetSelect(info.modeSelectionIndex == i);
	}

	std::string drawText = "";

	switch (info.reason) {
	case CantActReason_DontNeed:
		drawText = "すでにあなたの村です。";
		break;
	case CantActReason_NotEnoughMoney:
		drawText = "お金が足りません。";
		break;
	}

	dialog->SetText(drawText);
}
