/*
 * @brief ターンの実行処理
 * @author Sekino
 */

#pragma once
#ifndef _TURNPROCESSOR_H_
#define _TURNPROCESSOR_H_
#include <vector>
#include "Vector/Vector3.h"

enum MoveDirection {
	Move_Up,
	Move_Down,
	Move_Left,
	Move_Right
};

enum CommandType {
	Command_Move,
	Command_Item,
	Command_Magic,
};

enum TurnState {
	TurnState_CommandSelect,
	TurnState_Move,
	TurnState_Item,
	TurnState_Magic,
	TurnState_End
};

struct Command {
	CommandType type;
	std::string name;
	std::string explanation;
};

class PlayerData;
class MapManager;

class TurnProcessor {
private:
	// ターンの順番
	int currentTurn;
	PlayerData* currentPlayerData;

	MapManager* mapManager;
	// ターン順序
	std::vector<int> orderList;
	// 進めるマス
	std::vector<bool> canMoveTileList;
	// 進める回数
	int canMoveCount;
	// 進んだ回数
	int currentMoveCount;
	// 進んだマス
	std::vector<Vector3> movedTiles;
	// 選べるコマンドの配列
	std::vector<Command> commandList;
	// 選んでいるコマンド
	int selectCommand;
	// ターンの状態
	TurnState turnState;
public:
	TurnProcessor(MapManager* _mapManager);
	~TurnProcessor();

public:
	/*
	 * @brief ターンの順番を設定する
	 */
	void OrderSet(const std::vector<int>& _orderList) { orderList = _orderList; }

	/*
	 * @brief ターンの開始処理
	 */
	void TurnStart();

	/*
	 * @brief 移動可能なタイルを検索する
	 */
	void SearchCanMoveTiles();

	/*
	 * @brief ターンの移動処理
	 */
	void Move();

	/*
	 * @brief 止まったマスの効果発動
	 */
	void TileEffect(Vector3 _tilePosition);

	/*
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief ターンの描画
	 */
	void Render();
};

#endif