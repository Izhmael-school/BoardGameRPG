#include "VillagePart.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Input/InputManager.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "UI/Canvas/SceneCanvas/Village/VillageCanvas.h"
#include "UI/Canvas/UICanvasBase.h"
#include "Manager/UI/UImanager.h"

void VillagePart::Init(UIManager* _uiManager, std::function<void()> _leaveFunc) {
	uiManager = _uiManager;
	leaveFunc = _leaveFunc;

	auto data = MyJson::LoadJsonFile("res/ExternalFile/Data/OriginVillageData.json");

	for (auto& d : data) {
		VillageData v;
		v.name = d["name"];
		v.ownerPlayerNum = -1;
		v.rewardTax = 0;
		v.taxValue = d["tax"];
		v.villageLevel = 1;

		int posX = d["pos"][0];
		int posY = d["pos"][1];

		std::pair<int, int> pos = { posX,posY };

		std::pair<std::pair<int, int>, VillageData> addData = { pos,v };

		villageDataArray.emplace_back(addData);
	}

	villageCanvas = std::make_unique<VillageCanvas>();
	villageCanvas->Init();
}

void VillagePart::Update(float _t) {
	VillageNeedUIInfo info;

	reason = CantActReason_Invalid;

	switch (villageState) {
	case VillageState_Select:
		if (InputManager::GetInstance().IsKeyDown(KEY_LEFT))
			elementSelection = std::max(0, elementSelection - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RIGHT))
			elementSelection = std::min(elementSelection + 1, static_cast<int>(VillageState_Max) - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
			villageState = static_cast<VillageState>(elementSelection);
			modeSelection = 0;
		}
		break;
	case VillageState_GetTax:
		Update_GetTax(_t);
		break;
	case VillageState_Upgrade:
		Update_Upgrade(_t);
		break;
	case VillageState_Acquisition:
		Update_Acquisition(_t);
		break;
	case VillageState_Leave:
		Leave();
		break;
	}

	info.modeSelectionIndex = modeSelection;
	info.selectionIndex = elementSelection;
	info.villageData = currentVillage;
	info.villageState = villageState;
	info.reason = reason;

	static_cast<VillageCanvas*>(villageCanvas.get())->SetVillageNeedUIInfo(info);
}

void VillagePart::Enter(PlayerData* _currentTurnPlayer) {
	currentTurnPlayer = _currentTurnPlayer;
	// プレイヤーのデータがなければ帰る
	if (!currentTurnPlayer) {
		Leave();
		return;
	}

	Vector2 pos = currentTurnPlayer->GetMapPosition();
	// 座標から村データを探す
	bool isReturn = true;
	for (auto& v : villageDataArray) {
		int x = v.first.first;
		int y = v.first.second;

		if (x != pos.x || y != pos.y) continue;
		isReturn = false;
		currentVillage = &v.second;
		break;
	}

	// 指定の座標に村がなければ帰る
	if (isReturn) {
		Leave();
		return;
	}

	uiManager->PushCanvas(villageCanvas.get());

	villageState = VillageState_Select;

	// 誰も来たことがない村ならプレイヤーをオーナーにする
	if (currentVillage->ownerPlayerNum != -1)
		return;

	currentVillage->ownerPlayerNum = currentTurnPlayer->GetPlayerNum();
	// 税を貯めるようにする
	isSaveTax = true;
}

void VillagePart::Leave() {
	currentTurnPlayer = nullptr;
	currentVillage = nullptr;
	villageState = VillageState_Select;
	uiManager->PopCanvas(villageCanvas.get());
	leaveFunc();
}

void VillagePart::Update_GetTax(float _t) {
	static bool isBack = false;
	static CantActReason keepReason = CantActReason_Invalid;

	if (isBack && InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
		villageState = VillageState_Select;
		isBack = false;
		keepReason = CantActReason_Invalid;
		return;
	}

	if (isBack) {
		reason = keepReason;
		return;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_B)) {
		villageState = VillageState_Select;
		return;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_UP) || InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		modeSelection ^= 1;

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN) && modeSelection == 0) {
		GetTax();
		keepReason = reason;
		isBack = true;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN) && modeSelection == 1) {
		villageState = VillageState_Select;
	}
}

constexpr int VILLAGE_LEVEL_MAX = 3;
void VillagePart::Update_Upgrade(float _t) {
	static bool isBack = false;
	static CantActReason keepReason = CantActReason_Invalid;

	if (isBack && InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
		villageState = VillageState_Select;
		isBack = false;
		keepReason = CantActReason_Invalid;
		return;
	}

	if (isBack) {
		reason = keepReason;
		return;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_UP) || InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		modeSelection ^= 1;

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
		if (modeSelection == 0) {
			Upgrade();
			isBack = true;
			keepReason = reason;
		}
		else {
			villageState = VillageState_Select;
		}
	}
}

void VillagePart::Update_Acquisition(float _t) {
	static bool isBack = false;
	static CantActReason keepReason = CantActReason_Invalid;

	if (isBack && InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
		villageState = VillageState_Select;
		isBack = false;
		keepReason = CantActReason_Invalid;
		return;
	}

	if (isBack) {
		reason = keepReason;
		return;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_UP) || InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		modeSelection ^= 1;

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
		if (modeSelection == 0) {
			Acquisition();
			isBack = true;
			keepReason = reason;
		}
		else {
			villageState = VillageState_Select;
		}
	}
}

void VillagePart::GetTax() {
	if (!IsOwner()) {
		reason = CantActReason_DontOwner;
		return;
	}

	if (currentVillage->rewardTax <= 0) {
		reason = CantActReason_NotEnoughMoney;
		return;
	}

	// オーナーなら税を受け取る
	currentTurnPlayer->AddMoney(currentVillage->rewardTax);
	// 税をゼロにする
	currentVillage->rewardTax = 0;
}

void VillagePart::Upgrade() {
	// レベルが3なら最大のため帰る
	if (currentVillage->villageLevel == VILLAGE_LEVEL_MAX) {
		reason = CantActReason_DontNeed;
		return;
	}

	// 税5回分×1.2をアップグレード費用とする
	int upgradeCost = currentVillage->GetUpgradeCost();
	// お金が足りていればアップグレードする
	if (currentTurnPlayer->GetMoney() >= upgradeCost) {
		currentVillage->villageLevel += 1;
		// 1回で貯まる税が3倍になる
		currentVillage->taxValue *= 3;
	}
	else {
		reason = CantActReason_NotEnoughMoney;
	}
}

void VillagePart::Acquisition() {
	// 自分がオーナーなら帰る
	if (IsOwner()) {
		reason = CantActReason_DontNeed;
		return;
	}

	// 基本5000に村のレベル×5したものに1.5を掛ける
	int acquisitionCost = currentVillage->GetAcquisitionCost();

	// お金が足りていれば買収する
	if (currentTurnPlayer->GetMoney() >= acquisitionCost)
		currentVillage->ownerPlayerNum = currentTurnPlayer->GetPlayerNum();
	else
		reason = CantActReason_NotEnoughMoney;
}

bool VillagePart::IsOwner() { return currentTurnPlayer->GetPlayerNum() == currentVillage->ownerPlayerNum; }
