#include "ShopCanvas.h"
#include "UI/UIObject/UIText/UITextDialog/UITextDiaLog.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIButton/UIButton.h"
#include "UI/UIObject/UISprite/UISprite.h"
#include "UI/Input/UIInput.h"

void ShopCanvas::Init() {
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	dialog = Instantiate<UITextDialog>(explanation, Vector2(250, 700));
	dialog->SetActive(false);
	select = Instantiate<UIObject>();
	select->SetPosition(Vector2(0, 0));
	UITextStyle textStyle;
	textStyle.fontSize = 50;
	textStyle.textDrawPosition = TextDrawPosition_Center;
	textStyle.normalColor = 0x000000;

	int posX = 384;
	int posY = 800;

	auto buyButton = UniqueInstantiate<UIText>("買う", textStyle, Vector2(posX, posY));
	select->AddChild(std::move(buyButton));
	auto sellButton = UniqueInstantiate<UIText>("売る", textStyle, Vector2(posX * 2, posY));
	select->AddChild(std::move(sellButton));
	auto stealButton = UniqueInstantiate<UIText>("盗む", textStyle, Vector2(posX * 3, posY));
	select->AddChild(std::move(stealButton));
	auto leaveButton = UniqueInstantiate<UIText>("帰る", textStyle, Vector2(posX * 4, posY));
	select->AddChild(std::move(leaveButton));
}

void ShopCanvas::Update(float _t, UIInput& _input) {

	// 状態が変わったら前の状態のUIを閉じる
	if (shopState != prevShopState) {
		switch (prevShopState) {
		case ShopState_Selection:
			break;
		case ShopState_Buy:
			CloseBuyUI();
			break;
		case ShopState_Sell:
			CloseSellUI();
			break;
		case ShopState_Steal:
			break;
		case ShopState_Back:
			break;
		default:
			break;
		}

		prevShopState = shopState;
	}

	switch (shopState) {
	case ShopState_Selection:
	{
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

	case ShopState_Buy:
	{

		static bool isFirst = true;
		if (isFirst) {
			isFirst = false;
			OpenBuyUI();
		}

		UpdateBuyUI(_t, _input);


	}
	break;
	case ShopState_Sell:
	{

		static bool isFirst = true;
		if (isFirst) {
			isFirst = false;
			OpenSellUI();
		}

		UpdateSellUI(_t, _input);


		break;
	}
	case ShopState_Steal:
		break;
	case ShopState_Back:
		break;
	case ShopState_Max:
		break;
	default:
		break;
	}

	UICanvasBase::Update(_t, _input);
}

void ShopCanvas::OpenBuyUI() {
	// 既に開かれていたら帰る
	if (buy) return;

	buy = Instantiate<UIObject>();
	// 背景を追加
	auto bg = UniqueInstantiate<UISprite>(-1, Vector2(640, 50), 640.0f, 600.0f, 0.0f);
	buy->AddChild(std::move(bg));

	// アイテムを表示
	int startX = 650;
	int startY = 60;
	int spacingY = 60;
	UITextStyle itemStyle;
	itemStyle.fontSize = 30;
	itemStyle.normalColor = 0x000000;

	for (size_t i = 0; i < buyItems.size(); ++i) {
		const BuyElement& item = buyItems[i];
		std::string txt = item.elementName + " ";
		txt += std::to_string(item.value);

		auto textUi = UniqueInstantiate<UIText>(txt, itemStyle, Vector2(startX, startY + static_cast<int>(i) * spacingY));
		// 子に追加
		buy->AddChild(std::move(textUi));
	}

	// ダイアログを表示
	SetSelectVisible(false);

	elementIndex = 0;
}

void ShopCanvas::CloseBuyUI() {
	if (!buy) return;

	DeleteUI(buy);

	dialog->SetActive(false);
	SetSelectVisible(true);
}

void ShopCanvas::UpdateBuyUI(float _t, UIInput& _input) {
	if (!buy) return;

	std::vector<UIObject*> children = buy->GetChildren();

	static int prevElementIndex = -1;
	static DontBuyReason prevDontBuyReason = DontBuyReason_Invalid;

	if (elementIndex != prevElementIndex) {
		isDontBuyReasonTextVisible = false;
		prevDontBuyReason = DontBuyReason_Invalid;
	}

	for (size_t i = 0, max = children.size(); i < max; ++i) {
		UIObject* select = children[i];
		auto textUi = static_cast<UIText*>(select);
		if (!textUi) return;
		if (elementIndex != i - 1) {
			textUi->SetSelect(false);
			continue;
		}

		prevElementIndex = elementIndex;
		textUi->SetSelect(true);
		if (!isDontBuyReasonTextVisible)
			dialog->SetText(selectElementExplanation);
	}

	if (dontBuyReason != DontBuyReason_Invalid || isDontBuyReasonTextVisible) {
		std::string drawText = "";
		switch (prevDontBuyReason) {
		case DontBuyReason_CanBuy:
			drawText = "お買い上げありがとうございます。";
			break;
		case DontBuyReason_NoMoney:
			drawText = "お金が足りない。";
			break;
		case DontBuyReason_NoSpace:
			drawText = "これ以上持てない。";
			break;
		}
		dialog->SetText(drawText);
		if (!isDontBuyReasonTextVisible) {

			prevDontBuyReason = dontBuyReason;
			isDontBuyReasonTextVisible = true;
		}
	}


	//if (_input.IsKeyDown(KEY_B))
	//	CloseBuyUI();
}

void ShopCanvas::OpenSellUI() {
	// 既に開かれていたら帰る
	if (sell) return;

	sell = Instantiate<UIObject>();
	// 背景を追加
	auto bg = UniqueInstantiate<UISprite>(-1, Vector2(640, 50), 640.0f, 600.0f, 0.0f);
	sell->AddChild(std::move(bg));

	// アイテムを表示
	int startX = 650;
	int startY = 60;
	int spacingY = 60;
	UITextStyle itemStyle;
	itemStyle.fontSize = 30;
	itemStyle.normalColor = 0x000000;

	const auto& sellItems = getPlayerSellArrayFunc();

	for (size_t i = 0; i < sellItems.size(); ++i) {
		const SellElement& item = sellItems[i];
		std::string txt = item.elementName + " ";
		txt += std::to_string(item.value);

		auto textUi = UniqueInstantiate<UIText>(txt, itemStyle, Vector2(startX, startY + static_cast<int>(i) * spacingY));
		// 子に追加
		sell->AddChild(std::move(textUi));
	}

	// ダイアログを表示
	SetSelectVisible(false);

	elementIndex = 0;
}

void ShopCanvas::CloseSellUI() {
	if (!sell) return;

	DeleteUI(sell);

	dialog->SetActive(false);
	SetSelectVisible(true);
}

void ShopCanvas::UpdateSellUI(float _t, UIInput& _input) {
	if (!sell) return;

	std::vector<UIObject*> children = sell->GetChildren();

	static int prevElementIndex = -1;

	for (size_t i = 0, max = children.size(); i < max; ++i) {
		UIObject* select = children[i];
		auto textUi = static_cast<UIText*>(select);
		if (!textUi) return;
		if (elementIndex != i - 1) {
			textUi->SetSelect(false);
			continue;
		}

		prevElementIndex = elementIndex;
		textUi->SetSelect(true);

		dialog->SetText(selectElementExplanation);
	}
}

void ShopCanvas::ResetSellUI() {
	// 一度閉じて開きなおす
	CloseSellUI();
	OpenSellUI();
}
