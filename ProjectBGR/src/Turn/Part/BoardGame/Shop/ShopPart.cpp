#include "ShopPart.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "Manager/Data/ElementDataManager.h"
#include "Manager/Input/InputManager.h"
#include "Data/Player/PlayerData.h"
#include "UI/Canvas/UICanvasBase.h"
#include "UI/Canvas/SceneCanvas/Shop/ShopCanvas.h"
#include "Manager/UI/UIManager.h"
#include <format>
#include <algorithm>

void ShopPart::Init(UIManager* _uiManager, std::function<void()> _leaveFunc) {
	uiManager = _uiManager;
	leaveFunc = _leaveFunc;
	elementDataManager = &ElementDataManager::GetInstance();
	shopCanvas = std::make_unique<ShopCanvas>();
	shopCanvas->Init();
	auto shopCanvasPtr =  static_cast<ShopCanvas*>(shopCanvas.get());
	shopCanvasPtr->SetGetPlayerSellArrayFunc([this]() { return this->canSellArray; });
	sellUIResetFunc = [shopCanvasPtr]() {shopCanvasPtr->ResetSellUI();};
}

void ShopPart::Open(int _x, int _y, PlayerData* _currentTurnPlayer) {
	std::string path = std::format("res/ExternalFile/Data/ShopData/ShopData_{0}_{1}.json", _x, _y);
	auto data = MyJson::LoadJsonFile(path);

	// 買えるアイテムのデータを読み込む
	for (auto& d : data) {
		BuyElement elem;

		elem.id = d["id"];
		elem.elementID = static_cast<ElementID>(d["elementID"]);

		ElementDataBase ed;
		switch (elem.elementID) {
		case ElementID_Item:
			ed = elementDataManager->GetItemData(elem.id);
			break;
		case ElementID_Magic:
			ed = elementDataManager->GetMagicData(elem.id);
			break;
		case ElementID_Equip:
			EquipData eq = elementDataManager->GetEquipData(elem.id);
			ed = eq;
			elem.upStateValue = ed.effectValue;
			elem.equipSec = eq.equipSection;
			break;
		}

		elem.elementExplanation = ed.explanation;
		elem.elementName = ed.name;
		elem.value = ed.value;

		buyArray.emplace_back(elem);
	}
	shopState = ShopState_Selection;
	modeSelection = 0;
	uiManager->PushCanvas(shopCanvas.get());
	static_cast<ShopCanvas*>(shopCanvas.get())->SetBuyItems(buyArray);
	currentTurnPlayer = _currentTurnPlayer;
	if (!currentTurnPlayer) return;

	// あらかじめプレイヤーの持ち物を確認して売却用配列に入れる
	SearchSellElement();
}

void ShopPart::Close() {
	buyArray.clear();
	buyArray.shrink_to_fit();
}

void ShopPart::Update(float _t) {
	ShopUINeedInfo info;

	DontBuyReason reason = DontBuyReason_Invalid;
	switch (shopState) {
	case ShopState_Selection:
		if (InputManager::GetInstance().IsKeyDown(KEY_LEFT))
			modeSelection = std::max(0, modeSelection - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RIGHT))
			modeSelection = std::min(modeSelection + 1, static_cast<int>(ShopState_Max) - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RETURN))
			shopState = static_cast<ShopState>(modeSelection);
		break;
	case ShopState_Buy:
		if (InputManager::GetInstance().IsKeyDown(KEY_LEFT))
			elementSelection = std::max(0, elementSelection - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RIGHT))
			elementSelection = std::min(elementSelection + 1, static_cast<int>(buyArray.size()) - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RETURN))
			reason = Buy(elementSelection);
		if (InputManager::GetInstance().IsKeyDown(KEY_B))
			shopState = ShopState_Selection;

		info.selectElement = buyArray[elementSelection];
		break;
	case ShopState_Sell:
		if (InputManager::GetInstance().IsKeyDown(KEY_B))
			shopState = ShopState_Selection;

		if (canSellArray.empty()) return;

		if (InputManager::GetInstance().IsKeyDown(KEY_LEFT))
			elementSelection = std::max(0, elementSelection - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RIGHT))
			elementSelection = std::min(elementSelection + 1, static_cast<int>(canSellArray.size()) - 1);

			info.selectElement = canSellArray[elementSelection];

		if (InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
			const SellElement& sell = canSellArray[elementSelection];

			SellElement sold;
			sold.elementExplanation = "売りました。";
			info.selectElement = sold;

			Sell(sell);
			// 持ち物から消す
			switch (sell.elementID) {
			case ElementID_Item:
				currentTurnPlayer->RemoveItem(sell.baseArrayIndex);
				break;
			case ElementID_Magic:
			{
				currentTurnPlayer->RemoveMagic(sell.baseArrayIndex);
				break;
			}
			case ElementID_Equip:
			{
				currentTurnPlayer->RemoveEquip(sell.baseArrayIndex);
				break;
			}
			}
			// 配列から消す
			canSellArray.erase(std::remove_if(canSellArray.begin(), canSellArray.end(),
				[&](const SellElement& s) {
					return s.baseArrayIndex == sell.baseArrayIndex && s.id == sell.id && s.elementID == sell.elementID;
				}), canSellArray.end());

			// 売れるアイテムの更新
			SearchSellElement();
			// 売却UIのリセット
			sellUIResetFunc();
		}


		break;
	case ShopState_Steal:
		// いったん後回し
		break;
	case ShopState_Back:
		Leave();
		break;
	}

	
	info.selection = shopState;
	info.selectSectionIndex = modeSelection;
	info.elementIndex = elementSelection;
	
	info.dontBuyReason = reason;
	static_cast<ShopCanvas*>(shopCanvas.get())->SetNeedInfo(info);
}

DontBuyReason ShopPart::BuyQuery(int _elementIndex) {
	if (!currentTurnPlayer) return DontBuyReason_Invalid;

	DontBuyReason reason = DontBuyReason_CanBuy;

	// アイテムの場合の持ち物にスペースがあれば
	if (buyArray[_elementIndex].elementID == ElementID_Item && currentTurnPlayer->IsAddItem())
		reason = DontBuyReason_NoSpace;
	// 魔法の場合のスペースがあれば
	if (buyArray[_elementIndex].elementID == ElementID_Magic && currentTurnPlayer->IsAddMagic())
		reason = DontBuyReason_NoSpace;

	// 金が足りない場合は買えない
	if (buyArray[_elementIndex].value > currentTurnPlayer->GetMoney())
		reason = DontBuyReason_NoMoney;

	return reason;
}

DontBuyReason ShopPart::Buy(int _elementIndex) {
	// 購入金額がないなら帰る
	DontBuyReason reason = BuyQuery(_elementIndex);

	if (reason != DontBuyReason_CanBuy) return reason;

	BuyElement bought = buyArray[_elementIndex];
	// 金額を引く
	currentTurnPlayer->AddMoney(-bought.value);
	// アイテムを加える
	// 要改善:現在、装備は上書きされる
	switch (bought.elementID) {
	case ElementID_Item:
		currentTurnPlayer->AddItem(bought.id);
		break;
	case ElementID_Magic:
		currentTurnPlayer->AddMagic(bought.id);
		break;
	case ElementID_Equip:
		// 装備をしていれば売却する
		int beforeEquip = currentTurnPlayer->GetEquipList()[bought.equipSec];
		if (beforeEquip != -1)
			Sell(ElementID_Equip, beforeEquip);

		currentTurnPlayer->SetEquip(bought.id, bought.equipSec);
		break;
	}
	return DontBuyReason_CanBuy;
}

void ShopPart::Sell(ElementID _elementID, int _id) {
	ElementDataBase elem;
	int value;
	switch (_elementID) {
	case ElementID_Item:
		elem = elementDataManager->GetItemData(_id);
		break;
	case ElementID_Magic:
		elem = elementDataManager->GetMagicData(_id);
		break;
	case ElementID_Equip:
		elem = elementDataManager->GetEquipData(_id);
		break;
	}
	// 元値の半分にする
	value = elem.value / 2;
	currentTurnPlayer->AddMoney(value);
}

void ShopPart::Sell(SellElement _sell) {
	currentTurnPlayer->AddMoney(_sell.value);
}

void ShopPart::Leave() {
	// キャンバスの登録解除
	uiManager->PopCanvas(shopCanvas.get());
	// ショップ終了
	leaveFunc();
}

void ShopPart::SearchSellElement() {
	canSellArray.clear();

	std::vector<int> sellItemIDArray = currentTurnPlayer->GetItemList();
	std::vector<int> sellMagicIDArray = currentTurnPlayer->GetMagicList();
	std::vector<int> sellEquipIDArray = currentTurnPlayer->GetEquipList();
	int index = 0;
	// アイテム
	for (auto item : sellItemIDArray) {
		ElementDataBase elem = elementDataManager->GetItemData(item);
		SellElement sell;
		sell.id = item;
		sell.elementID = ElementID_Item;
		sell.elementName = elem.name;
		sell.elementExplanation = elem.explanation;
		// 価値は元の半分
		sell.value = elem.value / 2;
		sell.baseArrayIndex = index;
		index++;
		canSellArray.emplace_back(sell);
	}
	index = 0;
	// 魔法
	for (auto magic : sellMagicIDArray) {
		ElementDataBase elem = elementDataManager->GetMagicData(magic);
		SellElement sell;
		sell.id = magic;
		sell.elementID = ElementID_Magic;
		sell.elementName = elem.name;
		sell.elementExplanation = elem.explanation;
		// 価値は元の半分
		sell.value = elem.value / 2;
		sell.baseArrayIndex = index;
		index++;
		canSellArray.emplace_back(sell);
	}
	index = 0;
	// 装備
	for (auto equip : sellEquipIDArray) {
		ElementDataBase elem = elementDataManager->GetEquipData(equip);
		SellElement sell;
		sell.id = equip;
		sell.elementID = ElementID_Equip;
		sell.elementName = elem.name;
		sell.elementExplanation = elem.explanation;
		// 価値は元の半分
		sell.value = elem.value / 2;
		sell.baseArrayIndex = index;
		index++;
		canSellArray.emplace_back(sell);
	}
}

ShopPart::~ShopPart() = default;

