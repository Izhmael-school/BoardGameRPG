#include "TileEffect.h"
#include "Manager/Data/ElementDataManager.h"
#include <vector>
#include "Definition/CommonModule/Math/MyMath.h"
#include "Manager/Input/InputManager.h"
#include "Manager/Time/TimeManager.h"
#include "Data/Player/PlayerData.h"
#include "DxLib.h"

PlayerData* TileEffect::currentTurnPlayer = nullptr;
std::function<void(TurnState)> TileEffect::changeBoardGameState;
std::function<void(BoardGameUINeedInfo)> TileEffect::setBoardGameUINeedInfo;
bool TileEffect::isStop = false;

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
		changeBoardGameState(TurnState_ShopIn);
		isGoNext = true;
		return TileEffectEnum::TileEffect_ItemShop;
	case Tile_MagicShop:
		changeBoardGameState(TurnState_ShopIn);
		isGoNext = true;
		return TileEffectEnum::TileEffect_MagicShop;
	case Tile_EquipShop:
		changeBoardGameState(TurnState_ShopIn);
		isGoNext = true;
		return TileEffectEnum::TileEffect_EquipShop;
	case Tile_Village:
	{

		// 止まったマスにキャラクターがいれば戦闘に入る
		// 自身も含まれてる配列のためサイズは１より上の指定
		if (_characters.size() > 1) {
			isGoNext = true;
			return TileEffect_Battle;
		}
		// いなければそのまま村に入る
		else {
			changeBoardGameState(TurnState_VillageIn);
			isGoNext = true;
			return TileEffect_Village;
		}
	}
	case Tile_Church:
		changeBoardGameState(TurnState_ChurchIn);
		isGoNext = true;
		return TileEffectEnum::TileEffect_Church;
	default:
		isGoNext = true;
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

	float d = TimeManager::GetInstance().GetDeltaTime();

	static float time = 0;
	const float interval = 0.1f;
	time += d;

	if (time >= interval && !isStop) {
		time = 0;
		selectItem = MyMath::Random(0, max - 1);
	}

	if(isStop && InputManager::GetInstance().IsButtonDown(currentTurnPlayer->GetControllerNum(), XINPUT_A)) {
		isStop = false;
		itemArray.clear();
		itemArray.shrink_to_fit();
		selectItem = 0;
		return true;
	}

	// 確定
	if (InputManager::GetInstance().IsButtonDown(currentTurnPlayer->GetControllerNum(), XINPUT_A) && !isStop) {
		isStop = true;
		currentTurnPlayer->AddItem(itemArray[selectItem].id);
	}

	// UI用に情報を整理し、送る
	std::vector<std::string> itemNameArray;
	for (const auto& item : itemArray) {
		itemNameArray.emplace_back(item.name);
	}
	TileEffectUINeedInfo elementGetInfo = { selectItem, itemNameArray, isStop };
	BoardGameUINeedInfo info;
	info.info = elementGetInfo;
	info.state = TurnState_TileEffect;

	setBoardGameUINeedInfo(info);
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

	float d = TimeManager::GetInstance().GetDeltaTime();

	static float time = 0;
	const float interval = 0.1f;
	time += d;

	if (time >= interval && !isStop) {
		time = 0;
		selectMagic = MyMath::Random(0, max - 1);
	}

	if(isStop && InputManager::GetInstance().IsButtonDown(currentTurnPlayer->GetControllerNum(), XINPUT_A)) {
		isStop = false;
		magicArray.clear();
		magicArray.shrink_to_fit();
		selectMagic = 0;
		return true;
	}

	// 確定
	if (InputManager::GetInstance().IsButtonDown(currentTurnPlayer->GetControllerNum(), XINPUT_A) && !isStop) {
		isStop = true;
		currentTurnPlayer->AddMagic(magicArray[selectMagic].id);
	}

	// UI用に情報を整理し、送る
	std::vector<std::string> magicNameArray;
	for (const auto& magic : magicArray) {
		magicNameArray.emplace_back(magic.name);
	}
	TileEffectUINeedInfo elementGetInfo = { selectMagic, magicNameArray,isStop };
	BoardGameUINeedInfo info;
	info.info = elementGetInfo;
	info.state = TurnState_TileEffect;

	setBoardGameUINeedInfo(info);

	return false;
}
