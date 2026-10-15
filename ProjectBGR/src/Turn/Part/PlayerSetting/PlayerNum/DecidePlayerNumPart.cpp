#include "DecidePlayerNumPart.h"
#include "Manager/Input/InputManager.h"

void DecidePlayerNumPart::Update(float _t) {
	PlayerSettingBase::Update(_t);

	if(input->IsKeyDown(KEY_UP))
		currentSelect = std::max(0, currentSelect - 1);

	if (input->IsKeyDown(KEY_DOWN))
		currentSelect = std::min(currentSelect + 1, static_cast<int>(CanSelect_Max) - 1);
	
	switch (static_cast<CanSelect_DecidePlayerNumPart>(currentSelect)) {
	case CanSelect_Player:
		if (input->IsKeyDown(KEY_LEFT))
			playerNum = std::max(1, playerNum - 1);
		// CPUの数に応じて上限を下げる
		if (input->IsKeyDown(KEY_RIGHT))
			playerNum = std::min(playerNum + 1, PLAYER_MAX - cpuNum);

		break;
	case CanSelect_NPC:

		if (input->IsKeyDown(KEY_LEFT))
			cpuNum = std::max(0, cpuNum - 1);
		// プレイヤーの数に応じて上限を下げる
		if (input->IsKeyDown(KEY_RIGHT))
			cpuNum = std::min(cpuNum + 1, PLAYER_MAX - playerNum);

		break;
	case CanSelect_Next:
		// 参加人数が2人以上ならコントローラー設定に行く
		if (input->IsKeyDown(KEY_RETURN) && playerNum + cpuNum >= 2)
			changePartFunc(PlayerSettingPart_UsePortNum);

		break;
	}


	PlayerNumUINeedInfo playerNumInfo;
	playerNumInfo.playerNum = playerNum;
	playerNumInfo.cpuNum = cpuNum;
	playerNumInfo.currentSelect = currentSelect;
	PlayerSettingUINeedInfo info;
	info.info = playerNumInfo;
	info.part = PlayerSettingPart_PlayerNum;
	// キャンバスに情報を渡す
	setUIInfoFunc(info);
}

void DecidePlayerNumPart::Setup() {
	playerNum = 1;
	cpuNum = 1;
	currentSelect = 0;
}
