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

enum MoveDirection {
	Move_Up,
	Move_Down,
	Move_Left,
	Move_Right
};

class MoveCommand {
private:
	// コマンド選択に戻れるか
	bool isBack;

	// 進めるマス
	std::vector<bool> canMoveTileList;
	// 進める回数
	int canMoveCount;
	// 進んだ回数
	int currentMoveCount;
	// 進んだマス
	std::vector<Vector3> movedTiles;

public:
	/*
	 * @brief 移動
	 */
	void Move();

	/*
	 * @brief 移動できるマスを検索する
	 */
	void SearchCanMoveTiles();

	/*
	 * @brief 止まったマスの効果発動
	 */
	void TileEffect(Vector3 _tilePosition);
};

#endif