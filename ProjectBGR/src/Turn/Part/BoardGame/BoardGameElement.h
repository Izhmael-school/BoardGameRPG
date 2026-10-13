/*
 * @brief BoardGamePart.hで使っていた列挙体、構造体を分離したもの
 * @author Sekino
 */
#pragma once
#ifndef _BOARDGAMEELEMENT_H_
#define _BOARDGAMEELEMENT_H_

enum CommandType {
	Command_Move,
	Command_Item,
	Command_Magic,
	Command_SearchTile
};

enum TurnState {
	TurnState_CommandSelect,
	TurnState_Move,
	TurnState_Item,
	TurnState_Magic,
	TurnState_SearchTile,
	TurnState_TileEffect,
	TurnState_BattleIn,
	TurnState_ShopIn,
	TurnState_ChurchIn,
	TurnState_VillageIn,
	TurnState_End
};

#include <string>
struct Command {
	TurnState state;
	std::string name;
	std::string explanation;
};

#endif // !_BOARDGAMEELEMENT_H_