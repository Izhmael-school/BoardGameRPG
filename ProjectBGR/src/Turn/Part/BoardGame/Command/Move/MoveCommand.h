/*
 * @brief 移動を扱うコマンド
 * @author Sekino
 */

#pragma once
#ifndef _MOVECOMMAND_H_
#define _MOVECOMMAND_H_

#include "Vector/Vector3.h"
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
	// コマンド選択に戻れるか
	bool isBack = true;

	PlayerData* currentPlayerData = nullptr;
	// 進めるマス
	std::vector<bool> canMoveTileList;
	// ルーレットの数値
	int randCount = -1;
	// 
	bool confirmedRoulette = false;
	// 進める回数
	int canMoveCount = -1;
	// 進んだ回数
	int currentMoveCount = -1;
	// 進んだマス
	std::vector<Vector3> movedTiles;

	MapManager* mapManager = nullptr;
	// 移動を確定するかのコマンドID
	int confirmationMove;
	
private:
	/*
	 * @brief 移動
	 * @return 移動終了
	 */
	bool Move();

	/*
	 * @brief 一マス戻る
	 */
	void Back();

	/*
	 * @brief 移動できるマスを検索する
	 */
	void SearchCanMoveTiles();

	/*
	 * @brief 止まったマスの効果発動
	 */
	void TileEffect(Vector3 _tilePosition);

	/*
	 * @brief 進める回数を決める
	 */
	bool Roulette();

	/*
	 * @brief 移動を確定する
	 */
	bool ConfirmationMove();

public:
	/*
	 * @brief ルーレットを回して進むまでの一連の実行
	 */
	MoveCommandState Execute();

	/*
	 * @brief 初期化
	 */
	void Setup();

	/*
	 * @brief 描画
	 */
	void Render();

	/*
 * @brief ターンの開始時に現在のプレイヤーを渡してもらう
 */
	void SetCurrentTurnPlayer(PlayerData* _playerData) { currentPlayerData = _playerData; };

	void SetMapManager(MapManager* _map) { mapManager = _map; }

	int GetRandCount() const { return randCount; }

	int GetCanMoveCount() const { return canMoveCount; }

	int GetcurrentMoveCount() const { return currentMoveCount; }
};

#endif