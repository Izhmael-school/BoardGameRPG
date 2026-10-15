#include "PlayerSettingBase.h"
#include "Manager/Input/InputManager.h"

void PlayerSettingBase::Init(std::function<void(PlayerSettingPart)> _changePartFunc, std::function<void(const PlayerSettingUINeedInfo&)> _setUIInfoFunc) {
	changePartFunc = _changePartFunc;
	setUIInfoFunc = _setUIInfoFunc;
	input = &InputManager::GetInstance();
	Setup();
}

void PlayerSettingBase::Setup() {
}

void PlayerSettingBase::Update(float _t) {
	// ひとつ前に戻る
	if (input->IsKeyDown(KEY_B))
		changePartFunc(PlayerSettingPart_Back);
}
