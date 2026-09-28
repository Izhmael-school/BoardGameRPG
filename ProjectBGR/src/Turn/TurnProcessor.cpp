#include "TurnProcessor.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include "Manager/GameObject/GameObjectManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/UI/UIManager.h"
#include "UI/Canvas/SceneCanvas/BoardGame/BoardGameCanvas.h"

TurnProcessor::TurnProcessor(MapManager* _mapManager, CharacterDataManager* _characterDataManager, GameObjectManager* _gameObjectManager, UIManager* _uiManager)
	:currentTurn(0)
	, currentPlayerData(nullptr)
	, mapManager(_mapManager) 
	,character(_characterDataManager)
	,gameObjectManager(_gameObjectManager)
	,uiManager(_uiManager)
{
	std::vector<int> defaultOrderList = { 0, 1, 2, 3 };
	OrderSet(defaultOrderList);
	// キャンバスの生成
	boardGameCanvas = std::make_unique<BoardGameCanvas>();
	boardGameCanvas->Init();
	// ボードゲームの初期化
	boardGamePart.Init(_mapManager,character, [this]() {this->TurnEndFunc(); }, [this](CharacterData* _p1, CharacterData* _p2) {StartBattle(_p1, _p2); },_gameObjectManager,boardGameCanvas.get());
	// バトルの初期化
	battlePart.Init([this]() {this->TurnEndFunc(); character->DeleteEnemyData(); }, _gameObjectManager);
	// テスト：ボスをスポーンさせる
	character->CreateEnemyData(-1, 8, 9);
}

TurnProcessor::~TurnProcessor() {
}

void TurnProcessor::TurnStart() {
	currentPlayerData = character->GetPlayerData(orderList[currentTurn]);
	boardGamePart.SetCurrentPlayerData(currentPlayerData);
	boardGamePart.Setup();
	gamePart = GamePart_Board;
	GameObject* g = gameObjectManager->Find(currentPlayerData->GetPlayerName());
	g->GetTransform()->SetScale(1.0f);
	boardGameCanvas->SetCurrentTurnPlayerData(currentPlayerData);
	uiManager->PushCanvas(boardGameCanvas.get());
}

void TurnProcessor::Update(float _t) {
	switch (gamePart) {
	case GamePart_TurnStart:
		TurnStart();
		break;
	case GamePart_Board:
		boardGamePart.Update(_t);
		break;
	case GamePart_Battle:
		battlePart.Update(_t);
		break;
	case GamePart_TurnEnd:
		TurnEnd();
		break;
	}
}

void TurnProcessor::Render() {
	switch (gamePart) {
	case GamePart_Board:
		boardGamePart.Render();
		character->DebugRender();
		break;
	case GamePart_Battle:
		battlePart.Render();
		break;
	}
}

void TurnProcessor::TurnEnd() {
	// いったんキャンバスを消す
	uiManager->PopCanvas(boardGameCanvas.get());

	// 谺｡縺ｮ繧ｿ繝ｼ繝ｳ縺ｫ
	currentTurn++;
	if (currentTurn >= orderList.size())
		currentTurn = 0;

	gamePart = GamePart_TurnStart;

	GameObject* g = gameObjectManager->Find(currentPlayerData->GetPlayerName());
	g->GetTransform()->SetScale(0.5f);
}

void TurnProcessor::StartBattle(CharacterData* _p1, CharacterData* _p2) {
	// キャンバスの準備
	uiManager->PopCanvas(boardGameCanvas.get());
	uiManager->PushCanvas(nullptr);

	battlePart.BattleStart(_p1, _p2);
	gamePart = GamePart_Battle;
}

