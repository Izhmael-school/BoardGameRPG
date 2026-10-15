/*
 * @brief プレイヤーの設定に関す列挙体等をまとめたヘッダー
 */
#pragma once

enum PlayerSettingPart {
	PlayerSettingPart_Back = -2,
	PlayerSettingPart_Invalid = -1,
	PlayerSettingPart_PlayerNum,
	PlayerSettingPart_UsePortNum,
	PlayerSettingPart_PlayerName,
	PlayerSettingPart_TurnOrder,
	PlayerSettingPart_PlayerJob,
	PlayerSettingPart_Max
};

struct PlayerNumUINeedInfo{
	int playerNum = -1;
	int cpuNum = -1;
	int currentSelect = -1;
};


enum InputString {
	InputString_Hiragana,
	InputString_Katakana,
	InputString_English,
};

#include <vector>
#include <string>
struct PlayerNameUINeedInfo {
	int currentX = -1;
	int currentY = -1;
	InputString inputString = InputString_Hiragana;
	std::vector<std::string> name;
};

#include <variant>
struct PlayerSettingUINeedInfo{
	PlayerSettingPart part;
	std::variant<
		PlayerNumUINeedInfo,
		PlayerNameUINeedInfo
	> info;

	PlayerSettingUINeedInfo()
		: part(PlayerSettingPart_Invalid)
		, info(PlayerNumUINeedInfo{}) {
	}
};
