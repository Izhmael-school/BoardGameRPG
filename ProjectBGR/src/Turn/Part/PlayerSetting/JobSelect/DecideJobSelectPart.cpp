#include "DecideJobSelectPart.h"
#include "Definition/CommonModule/Math/MyMath.h"
#include "Manager/Input/InputManager.h"

void DecideJobSelectPart::Update(float _t) {

	// 登録しているプレイヤー数を超えたらCPUとして扱う
	if (playerNum <= currentCharacter)
		isCPU = true;

	if (isCPU) {
		if (!isEndCPUSelect) {
			isEndCPUSelect = CPUSelect();

			if (isEndCPUSelect && !isPlayerSelect) {

				currentSelectJobIndex = MyMath::Random(0, Job_Max);
				Correct();
			}
			// 入力が重ならないように一度帰る
			if (isEndCPUSelect)
				return;
		}
	}

	if (!isCPU || isPlayerSelect)
		Select();


	PlayerSettingUINeedInfo info;
	info.part = PlayerSettingPart_PlayerJob;
	JobSelectUINeedInfo jobSelectInfo;
	jobSelectInfo.currentPlayerIndex = currentCharacter;
	jobSelectInfo.currentSelectJobIndex = currentSelectJobIndex;
	jobSelectInfo.isEndCPUSelect = isEndCPUSelect;
	jobSelectInfo.cpuSelect = cpuSelect;
	jobSelectInfo.isCPU = isCPU;
	jobSelectInfo.characterNum = characterNum;
	jobSelectInfo.playerName = playerNameArray;
	info.info = jobSelectInfo;

	setUIInfoFunc(info);
}

void DecideJobSelectPart::Setup() {
	currentSelectJobIndex = 0;
	currentCharacter = 0;
	characterNum = 0;
	cpuSelect = 0;
	isEndCPUSelect = false;
	playerUsePortArray.clear();
	playerJobArray.clear();
}

void DecideJobSelectPart::Correct() {
	playerJobArray.emplace_back(static_cast<Job>(currentSelectJobIndex));
	currentCharacter++;
	currentSelectJobIndex = 0;
	isEndCPUSelect = false;

	// 未選択のプレイヤーがいれば続行
	if (currentCharacter == characterNum) {
		// 全員終わっていれば次に
		onGoNextFunc();
	}
}

bool DecideJobSelectPart::CPUSelect() {
	if (!isCPU) return false;

	if (InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_LEFT) || InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_RIGHT))
		cpuSelect ^= 1;

	// 決定
	if (InputManager::GetInstance().IsButtonDown(XINPUT_A)) {

		isPlayerSelect = static_cast<bool>(cpuSelect);
		cpuSelect = 0;
		return true;
	}
	return false;
}

void DecideJobSelectPart::Select() {
	int portIndex = 0;
	if (!isCPU)
		portIndex = playerUsePortArray[currentCharacter];

	if (InputManager::GetInstance().IsButtonDown(portIndex, XINPUT_DPAD_UP))
		currentSelectJobIndex = std::max(0, currentSelectJobIndex - 1);

	if (InputManager::GetInstance().IsButtonDown(portIndex, XINPUT_DPAD_DOWN))
		currentSelectJobIndex = std::min(static_cast<int>(Job_Max) - 1, currentSelectJobIndex + 1);

	if (InputManager::GetInstance().IsButtonDown(portIndex, XINPUT_A)) {
		Correct();
	}
}
