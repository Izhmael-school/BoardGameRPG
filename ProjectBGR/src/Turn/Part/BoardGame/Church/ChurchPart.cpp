#include "ChurchPart.h"
#include "Data/CharacterData.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Input/InputManager.h"
#include "Manager/UI/UIManager.h"
#include "UI/Canvas/SceneCanvas/Church/ChurchCanvas.h"

void ChurchPart::Init(UIManager* _uiManager, std::function<void()> _leaveFunc) {
	uiManager = _uiManager;
	leaveFunc = _leaveFunc;
	churchCanvas = std::make_unique<ChurchCanvas>();
	churchCanvas->Init();
	static_cast<ChurchCanvas*>(churchCanvas.get())->SetGetCanWarpChurchsFunc([this]() {return GetCanWarpChurchs(); });
	inputManager = &InputManager::GetInstance();
}

void ChurchPart::Open(CharacterData* _currentTurnPlayer) {
	currentTurnPlayer = _currentTurnPlayer;

	auto player = static_cast<PlayerData*>(currentTurnPlayer);
	if (player) {
		currentTurnPlayerPadIndex = player->GetControllerNum();
	}

	uiManager->PushCanvas(churchCanvas.get());

	churchState = ChurchState_Selection;

	SearchCanWarpChurch();
}

void ChurchPart::Close() {
	currentTurnPlayer = nullptr;
	churchState = ChurchState_Selection;
	uiManager->PopCanvas(churchCanvas.get());
	leaveFunc();
}

void ChurchPart::Update(float _t) {
	reason = DontReceiveServiceReason_Invalid;

	switch (churchState) {
	case ChurchState_Selection:
		Update_Selection(_t);
		break;
	case ChurchState_SetRP:
		Update_SetRP(_t);
		break;
	case ChurchState_Warp:
		Update_Warp(_t);
		break;
	case ChurchState_Recovery:
		Update_Recovery(_t);
		break;
	case ChurchState_Back:
		Close();
		break;
	}

	ChurchUINeedInfo info;
	info.selection = churchState;
	info.selectionSectionIndex = selection;
	info.dontReceiveServiceReason = reason;
	info.commandSelectIndex = commandSelect;
	auto church = static_cast<ChurchCanvas*>(churchCanvas.get());
	if (church)
		church->SetNeedInfo(info);
}

void ChurchPart::SetRespawnPoint() {
	currentTurnPlayer->SetRespawnPoint(currentTurnPlayer->GetMapPosition());
	reason = DontReceiveServiceReason_ReceiveService;
}
void ChurchPart::ChurchWarp(Vector2 _warpPos) {
	// お金がないなら帰る
	if (currentTurnPlayer->GetMoney() < WARP_NEED_MONEY) {
		reason = DontReceiveServiceReason_NoMoney;
		return;
	}
	// お金を減らす
	currentTurnPlayer->AddMoney(-WARP_NEED_MONEY);
	currentTurnPlayer->SetMapPosition(_warpPos.x, _warpPos.y);
}

void ChurchPart::SearchCanWarpChurch() {
	Vector2 pPos = currentTurnPlayer->GetMapPosition();

	int index = 0;
	for (int i = 0, max = CHURCH_NUM; i < max;i++) {
		int pos[2] = { CAN_WARP_CHURCH_POS[i][0],CAN_WARP_CHURCH_POS[i][1] };

		if (pos[0] != pPos.x && pos[1] != pPos.y) {

			ChurchWarpNeedInfo warpInfo = {Vector2(pos[0],pos[1]),CHURCH_NAME[index]};

			canWarpChurchs.emplace_back(warpInfo);
		}
		index++;
	}
}

void ChurchPart::HPRecovery() {
	// お金がないなら帰る
	if (currentTurnPlayer->GetMoney() < RECOVERY_NEED_MONEY) {
		reason = DontReceiveServiceReason_NoMoney;
		return;
	}
	// 体力が最大なら帰る
	if (currentTurnPlayer->GetCurrentHp() == currentTurnPlayer->GetMaxHp()) {
		reason = DontReceiveServiceReason_NotNeed;
		return;
	}

	// お金を減らす
	currentTurnPlayer->AddMoney(-RECOVERY_NEED_MONEY);
	currentTurnPlayer->SetCurrentHp(currentTurnPlayer->GetMaxHp());
}

void ChurchPart::Update_Selection(float _t) {

	if (inputManager->IsKeyDown(KEY_LEFT))
		selection = std::max(0, selection - 1);

	if (inputManager->IsKeyDown(KEY_RIGHT))
		selection = std::min(ChurchState_Max - 1, selection + 1);

	if (inputManager->IsKeyDown(KEY_RETURN)) {
		churchState = static_cast<ChurchState>(selection);
		commandSelect = 0;
	}
}

void ChurchPart::Update_SetRP(float _t) {
	static bool isBack = false;
	static DontReceiveServiceReason keepReason = DontReceiveServiceReason_Invalid;

	// フラグが立ったら帰れるようにする
	if (isBack && inputManager->IsKeyDown(KEY_RETURN)) {
		churchState = ChurchState_Selection;
		isBack = false;
		keepReason = DontReceiveServiceReason_Invalid;
		return;
	}

	// フラグが立ったまま帰っていないなら前のreasonを代入する
	if (isBack) {
		reason = keepReason;
		return;
	}

	if (inputManager->IsKeyDown(KEY_UP) || inputManager->IsKeyDown(KEY_DOWN))
		commandSelect = commandSelect ^ 1;

	// 0なら復活地点の変更。１なら戻る
	if (inputManager->IsKeyDown(KEY_RETURN) || inputManager->IsButtonDown(currentTurnPlayerPadIndex, XINPUT_A)) {
		if (commandSelect == 0)
			SetRespawnPoint();

		keepReason = reason;

		isBack = true;
	}
}


void ChurchPart::Update_Warp(float _t) {
	if (inputManager->IsKeyDown(KEY_B)) {
		// 戻る
		churchState = ChurchState_Selection;
	}

	if (inputManager->IsKeyDown(KEY_DOWN) || inputManager->IsButtonDown(currentTurnPlayerPadIndex, XINPUT_DPAD_DOWN))
		commandSelect = std::max(0, commandSelect - 1);

	if (inputManager->IsKeyDown(KEY_UP) || inputManager->IsButtonDown(currentTurnPlayerPadIndex, XINPUT_DPAD_UP))
		commandSelect = std::min(CHURCH_NUM - 1, commandSelect + 1);

	if (inputManager->IsKeyDown(KEY_RETURN) || inputManager->IsButtonDown(currentTurnPlayerPadIndex, XINPUT_A)) {
		int wx = CAN_WARP_CHURCH_POS[commandSelect][0];
		int wy = CAN_WARP_CHURCH_POS[commandSelect][1];
		ChurchWarp(Vector2(wx, wy));
		// ワープしたら強制的に教会を終わる
		churchState = ChurchState_Back;
	}
}

void ChurchPart::Update_Recovery(float _t) {
	static bool isBack = false;
	static DontReceiveServiceReason keepReason = DontReceiveServiceReason_Invalid;

	// フラグが立ったら帰れるようにする
	if (isBack && inputManager->IsKeyDown(KEY_RETURN)) {
		churchState = ChurchState_Selection;
		isBack = false;
		keepReason = DontReceiveServiceReason_Invalid;
		return;
	}

	// フラグが立ったまま帰っていないなら前のreasonを代入する
	if (isBack) {
		reason = keepReason;
		return;
	}

	if (inputManager->IsKeyDown(KEY_UP) || inputManager->IsKeyDown(KEY_DOWN))
		commandSelect = commandSelect ^ 1;

	// 0なら回復。１なら戻る
	if (inputManager->IsKeyDown(KEY_RETURN) || inputManager->IsButtonDown(currentTurnPlayerPadIndex, XINPUT_A)) {
		if (commandSelect == 0)
			HPRecovery();

		keepReason = reason;

		isBack = true;
	}
}
