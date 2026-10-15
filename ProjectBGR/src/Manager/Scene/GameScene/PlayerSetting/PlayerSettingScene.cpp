#include "PlayerSettingScene.h"
#include "UI/Canvas/SceneCanvas/PlayerSetting/PlayerSettingCanvas.h"
#include "Manager/UI/UIManager.h"


void PlayerSettingScene::ChangeSettingPart(PlayerSettingPart _part) {
	if (_part != PlayerSettingPart_Back)
		currentPart = _part;
	else
		// Backならひとつ前に戻る
		currentPart = static_cast<PlayerSettingPart>(currentPart - 1);
}

PlayerSettingScene::PlayerSettingScene(UIManager* _uiManager)
	:uiManager(_uiManager)
	,currentPart(PlayerSettingPart_PlayerNum)
{
	sceneCanvas = std::make_unique<PlayerSettingCanvas>();

	std::function<void(PlayerSettingPart)> changeSettingPart = [this](PlayerSettingPart _part) {ChangeSettingPart(_part); };
	std::function<void(PlayerSettingUINeedInfo)> setNeedInfo = [this](PlayerSettingUINeedInfo _info) {static_cast<PlayerSettingCanvas*>(sceneCanvas.get())->SetNeedInfo(_info); };

	playerNum.Init(changeSettingPart, setNeedInfo);
	playerName.Init(changeSettingPart, setNeedInfo);
}

PlayerSettingScene::~PlayerSettingScene() {
}

void PlayerSettingScene::Update(float _t) {
	switch (currentPart) {
	case PlayerSettingPart_Back:
		break;
	case PlayerSettingPart_PlayerNum:
		playerNum.Update(_t);
		break;
	case PlayerSettingPart_UsePortNum:
		playerName.Update(_t);
		break;
	case PlayerSettingPart_PlayerName:
		break;
	case PlayerSettingPart_TurnOrder:
		break;
	case PlayerSettingPart_PlayerJob:
		break;
	}
}

void PlayerSettingScene::Render() {
}

void PlayerSettingScene::Setup() {
	uiManager->PushCanvas(sceneCanvas.get());
}

void PlayerSettingScene::Cleanup() {
	uiManager->PopCanvas(sceneCanvas.get());
}
