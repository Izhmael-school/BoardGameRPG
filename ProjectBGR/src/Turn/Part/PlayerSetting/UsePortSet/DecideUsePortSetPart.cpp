#include "DecideUsePortSetPart.h"
#include "Manager/Input/InputManager.h"

void DecideUsePortSetPart::Update(float _t) {
	// 参加するプレイヤー分のコントローラが登録されたら次に行く
	if (playerUsePortArray.size() >= playerNum)
		onGoNextFunc();

	for (int i = 0, max = 4; i < max; i++) {
		// Aを押したコントローラを使用するボタンとして登録する
		if (!InputManager::GetInstance().IsButtonDown(i, XINPUT_A)) continue;
		playerUsePortArray.emplace_back(i);
		break;
	}

	PlayerSettingUINeedInfo info;
	info.part = PlayerSettingPart_UsePortSet;
	UsePortSetUINeedInfo thisInfo;
	thisInfo.playerName = playerName;
	thisInfo.playerNum = playerNum;
	thisInfo.playerUsePortArray = playerUsePortArray;
	thisInfo.characterNum = characterNum;
	info.info = thisInfo;

	setUIInfoFunc(info);
}

void DecideUsePortSetPart::Setup() {
	playerName.clear();
	playerNum = 0;
	playerUsePortArray.clear();
}
