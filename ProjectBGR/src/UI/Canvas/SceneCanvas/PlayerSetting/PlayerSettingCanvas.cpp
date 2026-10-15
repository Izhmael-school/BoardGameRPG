#include "PlayerSettingCanvas.h"
#include "UI/UIObject/UItext/UItext.h"

void PlayerSettingCanvas::Init() {
}

#include "Dxlib.h"
void PlayerSettingCanvas::Update(float _t, UIInput& _input) {
	PlayerSettingPart part = info.part;

	if (prevPart != part)
		switch (prevPart) {
		case PlayerSettingPart_PlayerNum:
			ClosePlayerNum();
			break;
		case PlayerSettingPart_UsePortNum:
			break;
		case PlayerSettingPart_PlayerName:
			break;
		case PlayerSettingPart_TurnOrder:
			break;
		case PlayerSettingPart_PlayerJob:
			break;
		}

	switch (part) {
	case PlayerSettingPart_PlayerNum:
		UpdatePlayerNum();
		break;
	case PlayerSettingPart_UsePortNum:
		break;
	case PlayerSettingPart_PlayerName:
		break;
	case PlayerSettingPart_TurnOrder:
		break;
	case PlayerSettingPart_PlayerJob:
		break;
	}
	DrawLine(960, 0, 960, 1080, 0x000000);
	prevPart = part;
}

void PlayerSettingCanvas::OpenPlayerNum() {
	// 既に開いていれば戻る
	if (playerNum) return;

	playerNum = Instantiate<UIObject>();

	UITextStyle titleStyle;
	titleStyle.textDrawPosition = TextDrawPosition_Center;
	titleStyle.fontSize = 50;

	int cw = 1920 / 2;

	auto titleText = UniqueInstantiate<UIText>("参加人数", titleStyle, Vector2(cw, 150));

	UITextStyle numStyle;
	numStyle.fontSize = 30;
	numStyle.textDrawPosition = TextDrawPosition_Center;
	auto playerText = UniqueInstantiate<UIText>("プレイヤーの数", numStyle, Vector2(cw, 400));
	auto playerNumText = UniqueInstantiate<UIText>("0", numStyle, Vector2(cw, 450));
	auto cpuText = UniqueInstantiate<UIText>("CPUの数", numStyle, Vector2(cw, 500));
	auto cpuNumText = UniqueInstantiate<UIText>("0", numStyle, Vector2(cw, 550));
	auto nextText = UniqueInstantiate<UIText>("決定", numStyle, Vector2(cw,700));

	playerNum->AddChild(std::move(titleText));
	playerNum->AddChild(std::move(playerText));
	playerNum->AddChild(std::move(playerNumText));
	playerNum->AddChild(std::move(cpuText));
	playerNum->AddChild(std::move(cpuNumText));
	playerNum->AddChild(std::move(nextText));
}

void PlayerSettingCanvas::ClosePlayerNum() {
}

constexpr int PLAYERNUMTEXT_INDEX = 2;
constexpr int CPUNUMTEXT_INDEX = 4;
void PlayerSettingCanvas::UpdatePlayerNum() {
	if (!playerNum)
		OpenPlayerNum();

	auto& playerNumInfo = std::get<PlayerNumUINeedInfo>(info.info);
	auto children = playerNum->GetChildren();
	for (int i = 0, max = children.size(); i < max;i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		if (1 + playerNumInfo.currentSelect * 2 != i)
			text->SetSelect(false);
		else
			text->SetSelect(true);

		if (i == PLAYERNUMTEXT_INDEX)
			text->SetText(std::to_string(playerNumInfo.playerNum));
		if (i == CPUNUMTEXT_INDEX)
			text->SetText(std::to_string(playerNumInfo.cpuNum));
	}
}

void PlayerSettingCanvas::OpenPlayerName() {
	if (playerName) return;

	playerName = Instantiate<UIObject>();

	
}

void PlayerSettingCanvas::ClosePlayerName() {
}

void PlayerSettingCanvas::UpdatePlayerName() {
	if (!playerName)
		OpenPlayerName();


}
