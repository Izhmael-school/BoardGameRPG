/*
 * @brief プレイヤーの設定に関す列挙体等をまとめたヘッダー
 */
#pragma once

enum PlayerSettingPart {
	PlayerSettingPart_Back = -2,
	PlayerSettingPart_Invalid = -1,
	PlayerSettingPart_PlayerNum,
	PlayerSettingPart_UsePortSet,
	PlayerSettingPart_PlayerName,
	PlayerSettingPart_TurnOrder,
	PlayerSettingPart_PlayerJob,
	PlayerSettingPart_Confirm,
	PlayerSettingPart_Max
};

struct PlayerNumUINeedInfo {
	int playerNum = -1;
	int cpuNum = -1;
	int currentSelect = -1;
};

#include <vector>
#include <string>
const std::vector<std::vector<std::string>> HIRAGANA = {
		{"あ", "い", "う", "え", "お","が", "ぎ", "ぐ", "げ", "ご"},
		{"か", "き", "く", "け", "こ","ざ", "じ", "ず", "ぜ", "ぞ"},
		{"さ", "し", "す", "せ", "そ","だ", "ぢ", "づ", "で", "ど"},
		{"た", "ち", "つ", "て", "と","ば", "び", "ぶ", "べ", "ぼ"},
		{"な", "に", "ぬ", "ね", "の","ぱ", "ぴ", "ぷ", "ぺ", "ぽ"},
		{"は", "ひ", "ふ", "へ", "ほ","ぁ", "ぃ", "ぅ", "ぇ", "ぉ"},
		{"ま", "み", "む", "め", "も","ゃ", "ゅ", "ょ", "<-", "<="},
		{"や", "ゐ", "ゆ", "ゑ", "よ", "", "", "", "", "ｶﾅ"},
		{"ら", "り", "る", "れ", "ろ", "", "", "", "", "英"},
		{"わ", "",   "を", "",   "ん", "", "", "", "", "OK"}
};
const std::vector<std::vector<std::string>> KATAKANA = {
	{"ア", "イ", "ウ", "エ", "オ", "ガ", "ギ", "グ", "ゲ", "ゴ"},
	{"カ", "キ", "ク", "ケ", "コ", "ザ", "ジ", "ズ", "ゼ", "ゾ"},
	{"サ", "シ", "ス", "セ", "ソ", "ダ", "ヂ", "ヅ", "デ", "ド"},
	{"タ", "チ", "ツ", "テ", "ト", "バ", "ビ", "ブ", "ベ", "ボ"},
	{"ナ", "ニ", "ヌ", "ネ", "ノ", "パ", "ピ", "プ", "ペ", "ポ"},
	{"ハ", "ヒ", "フ", "ヘ", "ホ", "ァ", "ィ", "ゥ", "ェ", "ォ"},
	{"マ", "ミ", "ム", "メ", "モ", "ャ", "ュ", "ョ", "<-", "<="},
	{"ヤ", "ヰ", "ユ", "ヱ", "ヨ", "ヴ", "", "", "", "ﾋﾗ"},
	{"ラ", "リ", "ル", "レ", "ロ", "", "", "", "", "英"},
	{"ワ", "",   "ヲ", "",   "ン", "", "", "", "", "OK"}
};

const std::vector<std::vector<std::string>> ENGLISH = {
	{"０", "１", "２", "３", "４", "５", "６", "７", "８", "９"},
	{"Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "ａ", "ｂ", "ｃ", "ｄ", "ｅ"},
	{"Ｆ", "Ｇ", "Ｈ", "Ｉ", "Ｊ", "ｆ", "ｇ", "ｈ", "ｉ", "ｊ"},
	{"Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ", "ｋ", "ｌ", "ｍ", "ｎ", "ｏ"},
	{"Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ", "ｐ", "ｑ", "ｒ", "ｓ", "ｔ"},
	{"Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ", "ｕ", "ｖ", "ｗ", "ｘ", "ｙ"},
	{"Ｚ", "ｚ", "",  "", "", "<-", "<=","ﾋﾗ", "ｶﾅ", "OK"}
};

#include "Vector2.h"
const std::vector<Vector2> HIRAGANA_COMMAND = {
	Vector2(-1,-1),
	Vector2(9,7),
	Vector2(7,6)
};

const std::vector<Vector2> KATAKANA_COMMAND = {
	Vector2(9,7),
	Vector2(-1,-1),
	Vector2(7,8)
};

const std::vector<Vector2> ENGLISH_COMMAND = {
	Vector2(9,8),
	Vector2(9,8),
	Vector2(-1,-1)
};

const std::vector<Vector2> COLLECT_COMMAND = {
	Vector2(9,9),
	Vector2(9,9),
	Vector2(9,6)
};

const std::vector<Vector2> ERASE_COMMAND = {
	Vector2(8,6),
	Vector2(8,6),
	Vector2(5,6)
};

const std::vector<Vector2> ALL_ERASE_COMMAND = {
	Vector2(9,6),
	Vector2(9,6),
	Vector2(6,6)
};

enum InputString {
	InputString_Hiragana,
	InputString_Katakana,
	InputString_English,
};

struct PlayerNameUINeedInfo {
	int currentX = -1;
	int currentY = -1;
	int playerNum = -1;
	InputString inputString = InputString_Hiragana;
	std::string name;
};

struct UsePortSetUINeedInfo {
	std::vector<std::string> playerName;
	std::vector<int> playerUsePortArray;
	int playerNum = -1;
	int characterNum = -1;
};

struct JobSelectUINeedInfo {
	int currentPlayerIndex = -1;
	int currentSelectJobIndex = -1;
	bool isCPU = false;
	bool isEndCPUSelect = false;
	int cpuSelect = -1;
	std::vector<std::string> playerName;
	int characterNum = -1;
};

enum TurnOrderSection {
	TurnOrderSection_ModeSelect = -1,
	TurnOrderSection_Random,
	TurnOrderSection_CharacterSelect,
	TurnOrderSection_Confirm
};

struct TurnOrderUINeedInfo {
	TurnOrderSection currentSection = TurnOrderSection_ModeSelect;
	int currentSelect = -1;
	int currentSelectCharacter = -1;
	int confirm = -1;
	std::vector<int> orderArray;
	std::vector<std::string> characterNameArray;
	int characterNum = -1;
};

#include <variant>
struct PlayerSettingUINeedInfo {
	PlayerSettingPart part;
	std::variant<
		PlayerNumUINeedInfo,
		PlayerNameUINeedInfo,
		UsePortSetUINeedInfo, 
		JobSelectUINeedInfo,
		TurnOrderUINeedInfo
	> info;

	PlayerSettingUINeedInfo()
		: part(PlayerSettingPart_Invalid)
		, info(PlayerNumUINeedInfo{}) {
	}
};
