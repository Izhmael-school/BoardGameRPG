#include "DecideTurnOrderPart.h"
#include "Manager/Input/InputManager.h"
#include "Definition/CommonModule/Math/MyMath.h"

void DecideTurnOrderPart::Update(float _t) {
	switch (section) {
	case TurnOrderSection_ModeSelect:
		if (InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_LEFT) || InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_RIGHT))
			currentSelect ^= 1;

		if (InputManager::GetInstance().IsButtonDown(XINPUT_A))
			section = static_cast<TurnOrderSection>(currentSelect);
		break;
	case TurnOrderSection_Random:
		RandomSelect();
		section = TurnOrderSection_Confirm;
		break;
	case TurnOrderSection_CharacterSelect:
		SelectMyself();
		break;
	case TurnOrderSection_Confirm:
		if (InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_LEFT) || InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_RIGHT))
			confirm ^= 1;

		if (InputManager::GetInstance().IsButtonDown(XINPUT_A))
			if (!confirm)
				Setup();
			else
				onGoNextFunc();
		break;
	}

	PlayerSettingUINeedInfo info;
	TurnOrderUINeedInfo turnOrderInfo;
	turnOrderInfo.characterNameArray = characterNameArray;
	turnOrderInfo.confirm = confirm;
	turnOrderInfo.currentSection = section;
	turnOrderInfo.currentSelect = currentSelect;
	turnOrderInfo.currentSelectCharacter = currentSelectPlayer;
	turnOrderInfo.orderArray = orderArray;
	turnOrderInfo.characterNum = characterNum;
	info.part = PlayerSettingPart_TurnOrder;
	info.info = turnOrderInfo;
	setUIInfoFunc(info);
}

void DecideTurnOrderPart::Setup() {
	section = TurnOrderSection_ModeSelect;
	orderArray.clear();
	orderArray.resize(characterNum);
	for (int i = 0; i < characterNum; i++) {
		orderArray[i] = -1;
	}
}

void DecideTurnOrderPart::RandomSelect() {
	std::vector<int> defaultOrder;

	for (int i = 0, max = characterNum; i < max;i++) {
		defaultOrder.emplace_back(i);
	}

	for (int i = 0, max = characterNum; i < max; i++) {
		int rand = MyMath::Random(0, defaultOrder.size() - 1);
		orderArray[i] = defaultOrder[rand];
		defaultOrder.erase(defaultOrder.begin() + rand);
	}
}

void DecideTurnOrderPart::SelectMyself() {
	if (InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_LEFT))
		currentSelectPlayer = std::max(0, currentSelectPlayer - 1);
	if (InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_RIGHT))
		currentSelectPlayer = std::min(characterNum - 1, currentSelectPlayer + 1);

	static int turn = 0;
	if (InputManager::GetInstance().IsButtonDown(XINPUT_A)) {

		// すでに順番が決まっていれば帰る
		if (orderArray[currentSelectPlayer] != -1) return;

		orderArray[currentSelectPlayer] = turn;
		turn++;

		if (turn >= characterNum) {
			section = TurnOrderSection_Confirm;
			turn = 0;
		}
	}
}
