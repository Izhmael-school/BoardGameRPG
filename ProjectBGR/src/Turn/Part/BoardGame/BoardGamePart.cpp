#include "BoardGamePart.h"
#include "Manager/Input/InputManager.h"
#include "DxLib.h"
#include "Data/Player/PlayerData.h"

void BoardGamePart::Setup() {
	// できうるコマンドを登録
	commandList.clear();
	commandList.push_back(Command(TurnState::TurnState_Move, "Move", "test"));
	moveCommand.Setup();
	if (currentPlayerData->GetItemCount() > 0) {
		commandList.push_back(Command(TurnState::TurnState_Item, "Item", "test"));
	}
	if (currentPlayerData->GetMagicCount() > 0) {
		commandList.push_back(Command(TurnState::TurnState_Magic, "Magic", "test"));
	}

	// アイテムの使用回数を初期化
	itemCommand.ResetItemUsed();
	// 魔法の使用回数を初期化
	magicCommand.ResetMagicUsed();

	turnState = TurnState::TurnState_CommandSelect;
}

void BoardGamePart::Update(float _t) {
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
	{

		MoveCommandState move = moveCommand.Execute();

		switch (move) {
		case MoveCommand_Back:
			turnState = TurnState_CommandSelect;
			break;
		case MoveCommand_Stay:
			break;
		case MoveCommand_MoveEnd:
			turnState = TurnState_End;
			break;
		}
	}
	break;
	case TurnState_Item:
		if (itemCommand.SelectItem())
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

void BoardGamePart::Render() {
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
		moveCommand.Render();
		break;
	case TurnState_Item:
		itemCommand.Render();
		break;
	case TurnState_Magic:
		magicCommand.Render();
		break;
	case TurnState_End:
		turnEndFunc();
		break;
	default:
		break;
	}
}

void BoardGamePart::SetCurrentPlayerData(PlayerData* _currentPlayerData) {
	currentPlayerData = _currentPlayerData;
	itemCommand.SetCurrentTurnPlayer(_currentPlayerData);
	magicCommand.SetCurrentTurnPlayer(_currentPlayerData);
	moveCommand.SetCurrentTurnPlayer(_currentPlayerData);
}

void BoardGamePart::SetMapManager(MapManager* _mapManager) {
	moveCommand.SetMapManager(_mapManager);
}
