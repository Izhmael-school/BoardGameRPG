#include "VillagePart.h"
#include "Data/Player/PlayerData.h";
#include "Manager/Input/InputManager.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "UI/Canvas/SceneCanvas/Village/VillageCanvas.h"
#include "UI/Canvas/UICanvasBase.h"
#include "Manager/UI/UImanager.h"

void VillagePart::Init(UIManager* _uiManager) {
	uiManager = _uiManager;

	auto data = MyJson::LoadJsonFile("res/Data/OriginVillageData.json");

	for (auto& d : data) {
		VillageData v;
		v.name = d["name"];
		v.ownerPlayerNum = -1;
		v.rewardTax = 0;
		v.taxValue = d["tax"];
		v.villageLevel = 1;

		villageDataArray.emplace_back(v);
	}

	villageCanvas = std::make_unique<VillageCanvas>();
	villageCanvas->Init();
}

void VillagePart::Update(float _t) {
	switch (villageState) {
	case VillageState_Select:		
		if (InputManager::GetInstance().IsKeyDown(KEY_LEFT))
		modeSelection = std::max(0, modeSelection - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RIGHT))
			modeSelection = std::min(modeSelection + 1, static_cast<int>(VillageState_Max) - 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {
			villageState = static_cast<VillageState>(modeSelection);
			elementSelection = 0;
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
	default:
		break;
	}
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
		currentVillage = v.second;
		break;
	}

	// 指定の座標に村がなければ帰る
	if (isReturn) {
		Leave();
		return;
	}

	uiManager->PushCanvas(villageCanvas.get());

	// 誰も来たことがない村ならプレイヤーをオーナーにする
	if (currentVillage.ownerPlayerNum != -1)
		return;

	currentVillage.ownerPlayerNum = currentTurnPlayer->GetPlayerNum();
	// 税を貯めるようにする
	isSaveTax = true;
}

void VillagePart::Leave() {
	uiManager->PopCanvas(villageCanvas.get());
}

void VillagePart::Update_GetTax(float _t) {
	if (!IsOwner()) {
		villageState = VillageState_Select;
		return;
	}

	if (currentVillage.rewardTax <= 0) {
		villageState = VillageState_Select;
		return;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_UP) || InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		modeSelection ^= 1;

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN) && modeSelection == 0) {
		GetTax();
		villageState = VillageState_Select;
	}
}

constexpr int VILLAGE_LEVEL_MAX = 3;
void VillagePart::Update_Upgrade(float _t) {
	// レベルが3なら最大のため帰る
	if (currentVillage.villageLevel == VILLAGE_LEVEL_MAX) {
		villageState = VillageState_Select;
		return;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_UP) || InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		modeSelection ^= 1;

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN) && modeSelection == 0) {
		Upgrade();
		villageState = VillageState_Select;
	}
}

void VillagePart::Update_Acquisition(float _t) {
	// 自分がオーナーなら帰る
	if (IsOwner()) return;

	if (InputManager::GetInstance().IsKeyDown(KEY_UP) || InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		modeSelection ^= 1;

	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN) && modeSelection == 0) {
		Acquisition();
		villageState = VillageState_Select;
	}
}

void VillagePart::GetTax() {
	// オーナーなら税を受け取る
	currentTurnPlayer->AddMoney(currentVillage.rewardTax);
	// 税をゼロにする
	currentVillage.rewardTax = 0;
}

void VillagePart::Upgrade() {
	// 税5回分×1.2をアップグレード費用とする
	int upgradeCost = (currentVillage.taxValue * 5) * 1.2f;
	// お金が足りていればアップグレードする
	if (currentTurnPlayer->GetMoney() >= upgradeCost) {
		currentVillage.villageLevel += 1;
		// 1回で貯まる税が3倍になる
		currentVillage.taxValue *= 3;
	}
}

void VillagePart::Acquisition() {
	// 基本5000に村のレベル×5したものに1.5を掛ける
	int acquisitionCost = (5000 * (currentVillage.villageLevel * 5)) * 1.5;

	// お金が足りていれば買収する
	if (currentTurnPlayer->GetMoney() >= acquisitionCost)
		currentVillage.ownerPlayerNum = currentTurnPlayer->GetPlayerNum();
}
