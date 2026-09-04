/*
 * @brief ターンの実行処理
 * @author Sekino
 */

#pragma once
#ifndef _TURNPROCESSOR_H_
#define _TURNPROCESSOR_H_
#include <vector>
#include "Vector/Vector3.h"
#include "Command/Item/ItemCommand.h"
#include "Command/Magic/FieldMagicCommand.h"



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

#include <string>

struct Command {
	TurnState state;
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
	// 選べるコマンドの配列
	std::vector<Command> commandList;
	// 選んでいるコマンド
	int selectCommand;
	// ターンの状態
	TurnState turnState;

	// アイテムコマンド
	ItemCommand itemCommand;
	// 魔法コマンド
	FieldMagicCommand magicCommand;
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
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief ターンの描画
	 */
	void Render();
};

#endif