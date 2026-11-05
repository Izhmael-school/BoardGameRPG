#include "DecidePlayerNamePart.h"
#include "Manager/Input/InputManager.h"
#include <algorithm>
#include "Vector2.h"

void DecidePlayerNamePart::Update(float _t) {
	auto stringArray = HIRAGANA;
	switch (currentInputString) {
	case InputString_Katakana:
		stringArray = KATAKANA;
		break;
	case InputString_English:
		stringArray = ENGLISH;
		break;
	}

	InputManager& input = InputManager::GetInstance();

	if (input.IsButtonDown(XINPUT_DPAD_DOWN))
		currentY = std::min(currentY + 1, static_cast<int>(stringArray.size() - 1));

	if (input.IsButtonDown(XINPUT_DPAD_UP))
		currentY = std::max(0, currentY - 1);

	if (input.IsButtonDown(XINPUT_DPAD_RIGHT))
		currentX = std::min(currentX + 1, static_cast<int>(stringArray[currentY].size() - 1));

	if (input.IsButtonDown(XINPUT_DPAD_LEFT))
		currentX = std::max(0, currentX - 1);

	// 空白のところに行かないようにクランプ
	currentX = std::clamp(currentX, 0, static_cast<int>(stringArray[currentY].size()) - 1);
	// やゆよやわをんの間の空白をよける
	while (stringArray[currentY][currentX] == "") {
		if (input.IsButtonDown(XINPUT_DPAD_DOWN) || input.IsButtonDown(XINPUT_DPAD_UP) || input.IsButtonDown(XINPUT_DPAD_LEFT))
			currentX--;
		else if (input.IsButtonDown(XINPUT_DPAD_RIGHT))
			currentX++;
	}

	static int playerNum = 0;

	// 確定
	if (input.IsButtonDown(XINPUT_A)) {
		std::string select = stringArray[currentY][currentX];

		if (HIRAGANA_COMMAND[currentInputString] == Vector2(currentX, currentY)) {
			currentInputString = InputString_Hiragana;
			currentX = 0, currentY = 0;
		}
		else if (KATAKANA_COMMAND[currentInputString] == Vector2(currentX, currentY)) {
			currentInputString = InputString_Katakana;
			currentX = 0, currentY = 0;
		}
		else if (ENGLISH_COMMAND[currentInputString] == Vector2(currentX, currentY)) {
			currentInputString = InputString_English;
			currentX = 0, currentY = 0;
		}
		else if (COLLECT_COMMAND[currentInputString] == Vector2(currentX, currentY)) {
			// 名前が入力されてなければ帰る
			if (!name.empty()) {
				onGoNextFunc();
				playerNum = (playerNum + 1) % 4;
			}
		}
		else if (ERASE_COMMAND[currentInputString] == Vector2(currentX, currentY)) {
			// 全角文字が配列3つ分だから三回消す
			for (int i = 0, max = 3; i < max; i++) {
				if (name.empty()) break;
				name.pop_back();
			}
		}
		else if (ALL_ERASE_COMMAND[currentInputString] == Vector2(currentX, currentY))
			name.clear();
		else {
			if (name.size() / 3 != nameMax) {
				// 全角文字が配列3つ分だから0~2までを入れる
				for (int i = 0, max = 3; i < max; i++) {
					name.push_back(stringArray[currentY][currentX][i]);
				}
			}
		}
	}

	PlayerNameUINeedInfo playerName;
	playerName.currentX = currentX;
	playerName.currentY = currentY;
	playerName.name = name;
	playerName.inputString = currentInputString;
	playerName.playerNum = playerNum;
	PlayerSettingUINeedInfo info;
	info.info = playerName;
	info.part = PlayerSettingPart_PlayerName;

	setUIInfoFunc(info);
}

void DecidePlayerNamePart::Setup() {
	name.clear();
	currentX = 0;
	currentY = 0;
	currentInputString = InputString_Hiragana;
}
