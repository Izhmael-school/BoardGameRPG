#include "TurnProcessor.h"
#include "Manager/PlayerData/PlayerDataManager.h"
#include "Data/Player/PlayerData.h"

TurnProcessor::TurnProcessor(MapManager* _mapManager)
	:currentTurn(0)
	, currentPlayerData(nullptr)
	, mapManager(_mapManager) {
	std::vector<int> defaultOrderList = { 0, 1, 2, 3 };
	OrderSet(defaultOrderList);
	boardGamePart.SetMapManager(_mapManager);
	boardGamePart.SetTurnEndFunc([this]() {this->TurnEnd(); });
}

TurnProcessor::~TurnProcessor() {
}

void TurnProcessor::TurnStart() {
	currentPlayerData = PlayerDataManager::GetInstance().GetPlayerData(orderList[currentTurn]);
	boardGamePart.SetCurrentPlayerData(currentPlayerData);
	boardGamePart.Setup();
	gamePart = GamePart_Board;
}

void TurnProcessor::Update(float _t) {
	switch (gamePart) {
	case GamePart_Board:
		boardGamePart.Update(_t);
		break;
	case GamePart_Battle:
		break;
	}
}

void TurnProcessor::Render() {
	switch (gamePart) {
	case GamePart_Board:
		boardGamePart.Render();
		break;
	case GamePart_Battle:
		break;
	}
}

void TurnProcessor::TurnEnd() {
	// 次のターンに
	currentTurn++;
	if (currentTurn >= orderList.size())
		currentTurn = 0;

	TurnStart();
}

