#include "TileEffect.h"
#include "Manager/Data/ElementDataManager.h"
#include <vector>
#include "Definition/CommonModule/Math/MyMath.h"
#include "Manager/Input/InputManager.h"
#include "Data/Player/PlayerData.h"
#include "DxLib.h"

PlayerData* TileEffect::currentTurnPlayer = nullptr;

TileEffectEnum TileEffect::Effect(MapTileType _type, std::vector<CharacterData*> _characters, bool& isGoNext) {
	isGoNext = false;
	switch (_type) {
	case Tile_Empty:
	{
		// 何もない場合は戦闘かイベント
		// アルファでは戦闘のみを実装

		// 止まったマスにキャラクターがいれば戦闘に入る
		// 自身も含まれてる配列のためサイズは１より上の指定
		if (_characters.size() > 1) {
			isGoNext = true;
			return TileEffect_Battle;
		}

		int rand = MyMath::Random(0, 100);

		// 75%で戦闘に入る
		if (rand <= BATTLE_IN_PERCENTAGE) {
			isGoNext = true;
			return TileEffect_Battle;
		}
		// 15%でイベントに入る(今は戦闘に入る)
		else if (BATTLE_IN_PERCENTAGE < rand && rand <= EVENT_IN_PERCENTAGE + BATTLE_IN_PERCENTAGE) {
			isGoNext = true;
			return TileEffect_Battle;
		}
		// どちらにも入らなければ何も起きない
		else {
			isGoNext = true;
			return TileEffect_Nothing;
		}

	}
	case Tile_Item:
		isGoNext = ItemGet();
		return TileEffectEnum::TileEffect_Item;
	case Tile_Magic:
		isGoNext = MagicGet();
		return TileEffectEnum::TileEffect_Magic;
	case Tile_ItemShop:
		return TileEffectEnum::TileEffect_ItemShop;
	case Tile_MagicShop:
		return TileEffectEnum::TileEffect_MagicShop;
	case Tile_EquipShop:
		return TileEffectEnum::TileEffect_EquipShop;
	case Tile_Village:
		return TileEffectEnum::TileEffect_Village;
	case Tile_Church:
		return TileEffectEnum::TileEffect_Church;
	default:
		return TileEffectEnum::TileEffect_Nothing;
	}

}

bool TileEffect::ItemGet() {
	// アイテムピックアップフラグ
	static std::vector<ItemData> itemArray;
	static int selectItem = 0;
	static int max = 10;

	// まだピックアップしてなければピックアップする
	if (itemArray.empty()) {
		ElementDataManager& data = ElementDataManager::GetInstance();
		int itemNum = data.GetItemDataNum();

		for (int i = 0; i < max; i++) {
			int rand = MyMath::Random(0, itemNum - 1);
			itemArray.push_back(data.GetItemData(rand));
		}
	}

	selectItem++;

	if (selectItem > max)
		selectItem = 0;

	// 確定
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {

		currentTurnPlayer->AddItem(itemArray[selectItem].id);
		selectItem = 0;
		itemArray.clear();
		itemArray.shrink_to_fit();
		return true;
	}
	return false;
}

bool TileEffect::MagicGet() {
	// アイテムピックアップフラグ
	static std::vector<MagicData> magicArray;
	static int selectMagic = 0;
	static int max = 10;

	// まだピックアップしてなければピックアップする
	if (magicArray.empty()) {
		ElementDataManager& data = ElementDataManager::GetInstance();
		int magicNum = data.GetMagicDataNum();

		for (int i = 0; i < max; i++) {
			int rand = MyMath::Random(0, magicNum - 1);
			magicArray.push_back(data.GetMagicData(rand));
		}
	}

	selectMagic++;

	if (selectMagic > max)
		selectMagic = 0;

	// 確定
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {

		currentTurnPlayer->AddMagic(magicArray[selectMagic].id);
		selectMagic = 0;
		magicArray.clear();
		magicArray.shrink_to_fit();
		return true;
	}
	return false;
}
