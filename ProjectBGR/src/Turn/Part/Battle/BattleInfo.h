#pragma once
#include "Data/CharacterData.h"
#include <vector>
#include <variant>
#include <string>
#include <functional>

class PlayerData;

enum BattleState {
	BattleState_Invalid = -1,
	BattleState_AttackOrder,
	BattleState_SelectCommand,
	BattleState_AttackConfirm,
	BattleState_TurnChange,
	BattleState_Result,
	BattleState_LevelUp,
	BattleState_TurnEnd,
	BattleState_Max
};

enum OffenseSelectCommand {
	OffenseCommand_Invaild = -1,
	OffenseCommand_Attack,
	OffenseCommand_Magic,
	OffenseCommand_FatalAttack,
	OffenseCommand_Max
};

enum DefenseSelectCommand {
	DefenseCommand_Invaild = -1,
	DefenseCommand_Guard,
	DefenseCommand_MagicGuard,
	DefenseCommand_Counter,
	DefenseCommand_Surrender,
	DefenseCommand_Max
};

struct AttackOrderUINeedInfo {
	int attackCharacterIndex;
};

struct SelectCommandUINeedInfo {
	std::vector<bool> isSelectedCommandArray;
	std::vector<std::string> offenseCommandName;
	std::vector<bool> offenseCanCommandSelect;
	std::vector<std::string> defenseCommandName;
	std::vector<bool> defenseCanCommandSelect;
};

struct AttackConfirmUINeedInfo {
	std::string offenseActionName;
	std::string defenseActionName;
	int damageValue;
};

struct TurnChangeUINeedInfo {
	int newAttackCharacterIndex = -1;
};

struct ResultUINeedInfo {
	int winPlayer = -1;
	int getMoney = -1;
	int getExp = -1;
	bool isSurrender = false;
	int removeMoney = -1;
	int removeExp = -1;
};

struct LevelUpUINeedInfo {
	PlayerData* levelUpPlayer = nullptr;
	int upLevelIndex = -1;
	int currentSelectIndex = -1;
	std::function<void()> startSortStatus;
	bool isPressA = false;
	int remainingPoint = -1;
	std::vector<int> upStatus = { -1,-1,-1,-1,-1 };
	bool isConfirm = false;
	int confirmIndex = -1;
};

struct BattleUINeedInfo {
	BattleState state = BattleState_Invalid;
	std::variant<AttackOrderUINeedInfo,
		SelectCommandUINeedInfo,
		AttackConfirmUINeedInfo,
		TurnChangeUINeedInfo,
		ResultUINeedInfo,
		LevelUpUINeedInfo
	> info;

	BattleUINeedInfo()
		: info(AttackOrderUINeedInfo{})
		, state(BattleState_Invalid) {
	}
};