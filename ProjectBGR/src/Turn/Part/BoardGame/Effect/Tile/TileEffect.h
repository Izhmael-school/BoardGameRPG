/*
 * @brief　マスの効果
 * @author Sekino
 */
#pragma once
#ifndef _TILEEFFECT_H_
#define _TILEEFFECT_H_

enum TileEffectEnum {
	TileEffect_Nothing,
	TileEffect_Battle,
	TileEffect_Event,
	TileEffect_Empty,
	TileEffect_ItemShop,
	TileEffect_EquipShop,
	TileEffect_MagicShop,
	TileEffect_Village,
	TileEffect_Church,
	TileEffect_Magic,
	TileEffect_Item
};

#include "Manager/Map/MapManager.h"
#include "../../BoardGameInfo.h"
#include <functional>
#include <vector>
#include "Data/CharacterData.h"

class PlayerData;

constexpr int BATTLE_IN_PERCENTAGE = 80;
constexpr int EVENT_IN_PERCENTAGE = 15;

class TileEffect {
private:
	static bool isStop;
	static PlayerData* currentTurnPlayer;
	static std::function<std::vector<CharacterData*>(int, int)> getTilePosCharacter;
	static std::function<void(TurnState)> changeBoardGameState;
	static std::function<void(BoardGameUINeedInfo)> setBoardGameUINeedInfo;
public:
	/*
	 * @biref 効果発動
	 * @param[out] isGoNext 次のステートに移行していいか
	 */
	static TileEffectEnum Effect(MapTileType _type,std::vector<CharacterData*> _characters,bool &isGoNext);

	static void SetCurrentTurnPlayer(PlayerData* _currentTurnPlayer) { currentTurnPlayer = _currentTurnPlayer; }

	static void Render();

	static void SetChangeTurnStateFunc(std::function<void(TurnState)> _changeFunc) { changeBoardGameState = _changeFunc; }

	static void SetSetBoardGameUINeedInfoFunc(std::function<void(BoardGameUINeedInfo)> _setFunc) { setBoardGameUINeedInfo = _setFunc; }

	static bool IsStop() { return isStop; }
private:
	/*
	 * @brief　全アイテムの中から10個重複ありでピックアップしてランダムで一つ入手
	 */
	static bool ItemGet();

	/*
	 * @brief　全魔法の中から10個重複ありでピックアップしてランダムで一つ入手
	 */
	static bool MagicGet();
};

#endif // !_TILEEFFECT_H_