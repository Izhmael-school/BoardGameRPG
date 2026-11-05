/*
 * @brief ボードゲームに関する列挙体等をまとめたヘッダー
 */
#pragma once
#include <variant>
#include <vector>

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

enum MoveDirection {
	Move_Up,
	Move_Down,
	Move_Left,
	Move_Right
};

enum MoveCommandState {
	MoveCommand_Back,
	MoveCommand_RouletteTime,
	MoveCommand_Move,
	MoveCommand_Confirmation,
	MoveCommand_MoveEnd,
};

#include <string>
struct Command {
	TurnState state;
	std::string name;
	std::string explanation;
};

struct PlayerUIInfo {
	std::string playerName;
	int atk;
	int def;
	int vit;
	int mag;
	int spd;
	int luk;
	int money;
	std::vector<int> itemList;
	std::vector<int> magicList;
	std::vector<int> equipList;
};

#include "Manager/Map/MapInfo.h"
class CharacterData;
struct SearchTileUINeedInfo {
	MapTileType tileType;
	std::vector<CharacterData*> characters;
};

struct BattleInUINeedInfo {
	std::vector<CharacterData*> battleTargets;
	int selectIndex;
};

struct TileEffectUINeedInfo {
	int currentSelect = -1;
	std::vector<std::string> elementNameList;
	bool isStop = false;
};

struct MoveCommandUINeedInfo {
	float angle = 0.0f;
	MoveCommandState commandState = MoveCommand_Back;
	bool isRouletteStop = false;
	int canMoveCount = -1;
	bool isDialogTextErase = false;
	int confirmationMove = -1;
};

struct CommandSelectUINeedInfo {
	std::vector<Command> commandList;
	int selectIndex = 0;
};

struct BoardGameUINeedInfo {
	std::variant<
		MoveCommandUINeedInfo,
		CommandSelectUINeedInfo,
		SearchTileUINeedInfo,
		BattleInUINeedInfo,
		TileEffectUINeedInfo
	> info;

	TurnState state;

	BoardGameUINeedInfo()
		: info(MoveCommandUINeedInfo{})
		,state(TurnState_CommandSelect)
	{
	}
};