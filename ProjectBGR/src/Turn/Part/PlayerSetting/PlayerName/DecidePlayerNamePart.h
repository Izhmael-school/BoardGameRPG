/*
 * @brief プレイヤーの名前の設定
 */
#pragma once
#include "../PlayerSettingBase.h"
#include "../PlayerSettingInfo.h"
#include <string>
#include <vector>

class DecidePlayerNamePart :public PlayerSettingBase {
private:
	const std::vector<std::vector<std::string>> HIRAGANA = {
		{"あ", "い", "う", "え", "お","が", "ぎ", "ぐ", "げ", "ご"},
		{"か", "き", "く", "け", "こ","ざ", "じ", "ず", "ぜ", "ぞ"},
		{"さ", "し", "す", "せ", "そ","だ", "ぢ", "づ", "で", "ど"},
		{"た", "ち", "つ", "て", "と","ば", "び", "ぶ", "べ", "ぼ"},
		{"な", "に", "ぬ", "ね", "の","ぱ", "ぴ", "ぷ", "ぺ", "ぽ"},
		{"は", "ひ", "ふ", "へ", "ほ","ぁ", "ぃ", "ぅ", "ぇ", "ぉ"},
		{"ま", "み", "む", "め", "も","ゃ", "ゅ", "ょ", "ゐ", "ゑ"},
		{"や", "",   "ゆ", "",   "よ"},
		{"ら", "り", "る", "れ", "ろ"},
		{"わ", "",   "を", "",   "ん"}
	};
	const std::vector<std::vector<std::string>> KATAKANA = {
		{"ア", "イ", "ウ", "エ", "オ", "ガ", "ギ", "グ", "ゲ", "ゴ"},
		{"カ", "キ", "ク", "ケ", "コ", "ザ", "ジ", "ズ", "ゼ", "ゾ"},
		{"サ", "シ", "ス", "セ", "ソ", "ダ", "ヂ", "ヅ", "デ", "ド"},
		{"タ", "チ", "ツ", "テ", "ト", "バ", "ビ", "ブ", "ベ", "ボ"},
		{"ナ", "ニ", "ヌ", "ネ", "ノ", "パ", "ピ", "プ", "ペ", "ポ"},
		{"ハ", "ヒ", "フ", "ヘ", "ホ", "ァ", "ィ", "ゥ", "ェ", "ォ"},
		{"マ", "ミ", "ム", "メ", "モ", "ャ", "ュ", "ョ", "ヴ", ""},
		{"ヤ", "ヰ", "ユ", "ヱ", "ヨ"},
		{"ラ", "リ", "ル", "レ", "ロ"},
		{"ワ", "",   "ヲ", "",   "ン"}
	};

	const std::vector<std::vector<std::string>> ENGLISH = {
		{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"},
		{"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T"},
		{"U", "V", "W", "X", "Y", "Z", "",  "",  "",  ""},
		{"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"},
		{"k", "l", "m", "n", "o", "p", "q", "r", "s", "t"},
		{"u", "v", "w", "x", "y", "z", "",  "",  "",  ""},
		{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}
	};
	int currentX = 0;
	int currentY = 0;
	InputString currentInputString = InputString_Hiragana;

	std::vector<std::string> name;
public:
	void Update(float _t) override;

	void Setup() override;
};

