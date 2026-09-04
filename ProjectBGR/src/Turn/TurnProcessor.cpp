#include "TurnProcessor.h"
#include "Manager/Input/InputManager.h"
#include "Manager/PlayerData/PlayerDataManager.h"
#include "Manager/Map/MapManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Data/ElementDataManager.h"
#include "DxLib.h"

TurnProcessor::TurnProcessor(MapManager* _mapManager)
	:currentTurn(0)
	, currentPlayerData(nullptr)
	, mapManager(_mapManager) {
	std::vector<int> defaultOrderList = { 0, 1, 2, 3 };
	OrderSet(defaultOrderList);
}

TurnProcessor::~TurnProcessor() {
}

void TurnProcessor::TurnStart() {
	currentPlayerData = PlayerDataManager::GetInstance().GetPlayerData(orderList[currentTurn]);
	canMoveCount = 3;
	currentMoveCount = 0;
	movedTiles.clear();
	// できうるコマンドを登録
	commandList.clear();
	commandList.push_back(Command(TurnState::TurnState_Move, "Move", "test"));
	if (currentPlayerData->GetItemCount() > 0) {
		commandList.push_back(Command(TurnState::TurnState_Item, "Item", "test"));
		itemCommand.SetCurrentTurnPlayer(currentPlayerData);
	}
	if (currentPlayerData->GetMagicCount() > 0) {
		commandList.push_back(Command(TurnState::TurnState_Magic, "Magic", "test"));
		magicCommand.SetCurrentTurnPlayer(currentPlayerData);
	}

	// アイテムの使用回数を初期化
	itemCommand.ResetItemUsed();
	// 魔法の使用回数を初期化
	magicCommand.ResetMagicUsed();

	turnState = TurnState::TurnState_CommandSelect;
}

void TurnProcessor::Update(float _t) {
	switch (turnState) {
	case TurnState_CommandSelect:

		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
			selectCommand = max(0, selectCommand - 1);

		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
			selectCommand = min(static_cast<int>(commandList.size()) - 1, selectCommand + 1);
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
			turnState = commandList[selectCommand].state;
		}
		break;
	case TurnState_Move:

		break;
	case TurnState_Item:
		if(itemCommand.SelectItem())
			turnState = TurnState_CommandSelect;
		break;
	case TurnState_Magic:
		if (magicCommand.SelectMagic())
			turnState = TurnState_CommandSelect;
		break;
	case TurnState_End:
		break;
	default:
		break;
	}

}

void TurnProcessor::Render() {
	printfDx(currentPlayerData->GetPlayerName().c_str());
	printfDx("\nAtk: %d", currentPlayerData->GetAtk());
	printfDx("\nDef: %d", currentPlayerData->GetDef());
	printfDx("\nHP: %d", currentPlayerData->GetCurrentHp());
	printfDx("\nMag: %d", currentPlayerData->GetMag());
	printfDx("\nSpd: %d", currentPlayerData->GetSpd());
	printfDx("\nVit: %d", currentPlayerData->GetVit());
	printfDx("\nLuk: %d", currentPlayerData->GetLuk());
	printfDx("\nGold: %d", currentPlayerData->GetMoney());
	printfDx("\nLevel: %d", currentPlayerData->GetLevel());

	switch (turnState) {
	case TurnState_CommandSelect:
	{
		int i = 0;
		unsigned int color = 0xffffff;
		for (auto& command : commandList) {
			color = (i == selectCommand) ? 0xffff00 : 0x000000;
			DrawFormatString(400, 100 + i * 20, color, "%s: %s", command.name.c_str(), command.explanation.c_str());
			i++;
		}
	}
	break;
	case TurnState_Move:
		break;
	case TurnState_Item:
		itemCommand.Render();
		break;
	case TurnState_Magic:
		magicCommand.Render();
		break;
	case TurnState_End:
		break;
	default:
		break;
	}
}
