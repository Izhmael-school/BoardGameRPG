#include "DecidePlayerNamePart.h"
#include "Manager/Input/InputManager.h"

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

	if (InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		currentY = std::min(currentY + 1, static_cast<int>(stringArray.size()));

	if (InputManager::GetInstance().IsKeyDown(KEY_UP))
		currentY = std::max(0, currentY - 1);

	if (InputManager::GetInstance().IsKeyDown(KEY_RIGHT))
		currentX = std::min(currentX + 1, static_cast<int>(stringArray[currentY].size()));

	if (InputManager::GetInstance().IsKeyDown(KEY_LEFT))
		currentX = std::max(0, currentX - 1);

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN))
		name.emplace_back(stringArray[currentY][currentX]);

	PlayerNameUINeedInfo playerName;
	playerName.currentX = currentX;
	playerName.currentY = currentY;
	playerName.name = name;
	playerName.inputString = currentInputString;

	PlayerSettingUINeedInfo info;
	info.info = playerName;
	info.part = PlayerSettingPart_PlayerName;

	setUIInfoFunc(info);
}