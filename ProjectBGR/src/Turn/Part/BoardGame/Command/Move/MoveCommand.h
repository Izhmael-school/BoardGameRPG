/*
 * @brief 遘ｻ蜍輔ｒ謇ｱ縺・さ繝槭Φ繝・
 * @author Sekino
 */

#pragma once
#ifndef _MOVECOMMAND_H_
#define _MOVECOMMAND_H_

#include "Vector3.h"
#include <vector>

class PlayerData;
class MapManager;

enum MoveDirection {
	Move_Up,
	Move_Down,
	Move_Left,
	Move_Right
};

enum MoveCommandState {
	MoveCommand_Back,
	MoveCommand_Stay,
	MoveCommand_Move,
	MoveCommand_Confirmation,
	MoveCommand_MoveEnd,
};

class MoveCommand {
private:
	MoveCommandState commandState;
	// 繧ｳ繝槭Φ繝蛾∈謚槭↓謌ｻ繧後ｋ縺・
	bool isBack = true;

	PlayerData* currentPlayerData = nullptr;
	// 騾ｲ繧√ｋ繝槭せ
	std::vector<bool> canMoveTileList;
	// 繝ｫ繝ｼ繝ｬ繝・ヨ縺ｮ謨ｰ蛟､
	int randCount = -1;
	// 
	bool confirmedRoulette = false;
	// 騾ｲ繧√ｋ蝗樊焚
	int canMoveCount = -1;
	// 騾ｲ繧薙□蝗樊焚
	int currentMoveCount = -1;
	// 騾ｲ繧薙□繝槭せ
	std::vector<Vector3> movedTiles;

	MapManager* mapManager = nullptr;
	// 遘ｻ蜍輔ｒ遒ｺ螳壹☆繧九°縺ｮ繧ｳ繝槭Φ繝迂D
	int confirmationMove;
	
private:
	/*
	 * @brief 遘ｻ蜍・
	 * @return 遘ｻ蜍慕ｵゆｺ・
	 */
	bool Move();

	/*
	 * @brief 荳繝槭せ謌ｻ繧・
	 */
	void Back();

	/*
	 * @brief 遘ｻ蜍輔〒縺阪ｋ繝槭せ繧呈､懃ｴ｢縺吶ｋ
	 */
	void SearchCanMoveTiles();

	/*
	 * @brief 騾ｲ繧√ｋ蝗樊焚繧呈ｱｺ繧√ｋ
	 */
	bool Roulette();

	/*
	 * @brief 遘ｻ蜍輔ｒ遒ｺ螳壹☆繧・
	 */
	bool ConfirmationMove();

public:
	/*
	 * @brief 繝ｫ繝ｼ繝ｬ繝・ヨ繧貞屓縺励※騾ｲ繧縺ｾ縺ｧ縺ｮ荳騾｣縺ｮ螳溯｡・
	 */
	MoveCommandState Execute();

	/*
	 * @brief 蛻晄悄蛹・
	 */
	void Setup();

	/*
	 * @brief 謠冗判
	 */
	void Render();

	/*
 * @brief 繧ｿ繝ｼ繝ｳ縺ｮ髢句ｧ区凾縺ｫ迴ｾ蝨ｨ縺ｮ繝励Ξ繧､繝､繝ｼ繧呈ｸ｡縺励※繧ゅｉ縺・
 */
	void SetCurrentTurnPlayer(PlayerData* _playerData) { currentPlayerData = _playerData; };

	void SetMapManager(MapManager* _map) { mapManager = _map; }

	int GetRandCount() const { return randCount; }

	int GetCanMoveCount() const { return canMoveCount; }

	int GetcurrentMoveCount() const { return currentMoveCount; }
};

#endif