/*
 * @brief すごろくパート
 * @author Sekino
 */
#pragma once
#ifndef _BOARDGAMEPART_H_
#define _BOARDGAMEPART_H_

#include "Command/Item/ItemCommand.h"
#include "Command/Move/MoveCommand.h"
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

#include <functional>
class BoardGamePart {
private:
	// 現在のプレイヤーのデータ
	PlayerData* currentPlayerData;

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
	// 移動コマンド
	MoveCommand moveCommand;

	// ターンの終了処理
	std::function<void(void)> turnEndFunc;
public:
	void Setup();

	void Update(float _t);

	void Render();

	void SetCurrentPlayerData(PlayerData* _currentPlayerData);

	void SetMapManager(MapManager* _mapManager);

	void SetTurnEndFunc(std::function<void(void)> _turnEndFunc) { turnEndFunc = _turnEndFunc; }
};

#endif