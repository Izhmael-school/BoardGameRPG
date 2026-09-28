#include "BoardGameCanvas.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIButton/UIButton.h"
#include "UI/UIObject/UISprite/UISprite.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Data/ElementDataManager.h"
#include <format>
#include <algorithm>

void BoardGameCanvas::Init() {
	UITextStyle explanation;
	explanation.normalColor = 0x000000;
	explanation.fontSize = 50;
	auto e = UniqueInstantiate<UIText>("説明とかが\n入るところ", explanation, Vector2(10, 20));
	explanationUI = e.get();
	auto back = Instantiate<UISprite>(-1, Vector2(250, 700), 1420, 320);
	back->AddChild(std::move(e));
	UITextStyle playerName;
	playerName.fontSize = 50;
	playerNameUI = Instantiate<UIText>("PlayerName", playerName, Vector2(50, 100));
	moneyUI = Instantiate<UIText>("money", playerName, Vector2(50, 150));
}

void BoardGameCanvas::Update(float _t, UIInput& _input) {
	explanationUI->SetText("");

	playerNameUI->SetText(info.playerName);
	moneyUI->SetText(std::format("所持金  {}Ｇ", info.money));

	// アイテムリストの描画更新
	UpdateItemListUI();
	// マジックリストの描画更新
	UpdateMagicListUI();

	UICanvasBase::Update(_t, _input);
}

void BoardGameCanvas::SetCurrentTurnPlayerData(PlayerData* _currentTurnPlayer) {
	PlayerData* p = _currentTurnPlayer;
	info.playerName = p->GetPlayerName();
	info.atk = p->GetAtk();
	info.def = p->GetDef();
	info.vit = p->GetVit();
	info.mag = p->GetMag();
	info.spd = p->GetSpd();
	info.luk = p->GetLuk();
	info.itemList = p->GetItemList();
	info.magicList = p->GetMagicList();
	info.equipList = p->GetEquipList();
}

void BoardGameCanvas::OpenItemListUI() {
	if (itemListUI) return;

	Vector2 offset = Vector2(400, 200);
	Vector2 offset2 = Vector2(700, 400);
	itemListUI = Instantiate<UISprite>(-1, offset, offset2.x - offset.x, offset2.y - offset.y);
	int i = 0;
	unsigned int color = 0xffffff;
	UITextStyle itemUIStyle;
	itemUIStyle.normalColor = 0x000000;
	itemUIStyle.fontSize = 20;
	for (auto& item : info.itemList) {
		ItemData itemData = ElementDataManager::GetInstance().GetItemData(item);
		// 隱ｬ譏取枚縺縺代ｂ繧峨≧
		int x = (offset.x + (itemUIStyle.fontSize * ((i + 1) % 2))) + (((offset2.x - offset.x) / 2) * (i % 2));
		int y = (offset.y + itemUIStyle.fontSize) + (itemUIStyle.fontSize * (i / 2));

		Vector2 pPos = itemListUI->GetPosition();

		itemListUI->AddChild(UniqueInstantiate<UIText>(itemData.name, itemUIStyle, Vector2(x - pPos.x, y - pPos.y)));
		i++;
	}
}

void BoardGameCanvas::CloseItemListUI() {
	if (!itemListUI) return;

	// 生ポインタとコンテナ要素を比較するため find_if を使う
	auto itr = std::find_if(uiArray.begin(), uiArray.end(),
		[&](const auto& p) { return p.get() == itemListUI; });

	if (itr == uiArray.end()) return;

	// 所有権を解放してからコンテナから削除
	(*itr).reset();
	uiArray.erase(itr);
	itemListUI = nullptr;
}

void BoardGameCanvas::UpdateItemListUI() {
	if (!itemListUI) return;

	auto children = itemListUI->GetChildren();
	int i = 0;
	for (auto& child : children) {
		UIText* text = static_cast<UIText*>(child);

		if (i != selectIndex)
			text->SetSelect(false);
		else {
			text->SetSelect(true);
			std::string itemExplanation = ElementDataManager::GetInstance().GetItemData(info.itemList[i]).explanation;
			explanationUI->SetText(itemExplanation);
		}

		i++;
	}

}

void BoardGameCanvas::OpenMagicListUI() {
	if (magicListUI) return;

	Vector2 offset = Vector2(400, 200);
	Vector2 offset2 = Vector2(700, 400);
	magicListUI = Instantiate<UISprite>(-1, offset, offset2.x - offset.x, offset2.y - offset.y);
	int i = 0;
	unsigned int color = 0xffffff;
	UITextStyle magicUIStyle;
	magicUIStyle.normalColor = 0x000000;
	magicUIStyle.fontSize = 20;
	for (auto& magic : info.magicList) {
		MagicData magicData = ElementDataManager::GetInstance().GetMagicData(magic);
		// 隱ｬ譏取枚縺縺代ｂ繧峨≧
		int x = (offset.x + (magicUIStyle.fontSize * ((i + 1) % 2))) + (((offset2.x - offset.x) / 2) * (i % 2));
		int y = (offset.y + magicUIStyle.fontSize) + (magicUIStyle.fontSize * (i / 2));

		Vector2 pPos = magicListUI->GetPosition();

		magicListUI->AddChild(UniqueInstantiate<UIText>(magicData.name, magicUIStyle, Vector2(x - pPos.x, y - pPos.y)));
		i++;
	}
}

void BoardGameCanvas::CloseMagicListUI() {
	if (!magicListUI) return;

	// 生ポインタとコンテナ要素を比較するため find_if を使う
	auto itr = std::find_if(uiArray.begin(), uiArray.end(),
		[&](const auto& p) { return p.get() == magicListUI; });

	if (itr == uiArray.end()) return;

	// 所有権を解放してからコンテナから削除
	(*itr).reset();
	uiArray.erase(itr);
	magicListUI = nullptr;
}

void BoardGameCanvas::UpdateMagicListUI() {
	if (!magicListUI) return;

	auto children = magicListUI->GetChildren();
	int i = 0;
	for (auto& child : children) {
		UIText* text = static_cast<UIText*>(child);

		if (i != selectIndex)
			text->SetSelect(false);
		else {
			text->SetSelect(true);
			std::string magicExplanation = ElementDataManager::GetInstance().GetMagicData(info.magicList[i]).explanation;
			explanationUI->SetText(magicExplanation);
		}

		i++;
	}

}
