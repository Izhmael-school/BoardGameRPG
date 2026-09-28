#include "BoardGamePart.h"
#include "Manager/Input/InputManager.h"
#include "DxLib.h"
#include "Data/Player/PlayerData.h"
#include "Data/CharacterData.h"
#include "Data/Enemy/EnemyData.h"
#include "Manager/Map/MapManager.h"
#include "Effect/Tile/TileEffect.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include "GameObject/GameObject.h" 
#include "Manager/GameObject/GameObjectManager.h"
#include "UI/Canvas/SceneCanvas/BoardGame/BoardGameCanvas.h"

void BoardGamePart::Init(MapManager* _map, CharacterDataManager* _characterDataManager, std::function<void()> _turnEndFunc, std::function<void(CharacterData*, CharacterData*)> _battleStartFunc, GameObjectManager* _gameObjectManager, BoardGameCanvas* _canvas)
{
	canvas = _canvas;
	moveCommand.SetMapManager(_map);
	searchTileCommand.Init(_map, _characterDataManager);
	map = _map;
	turnEndFunc = _turnEndFunc;
	battleStartFunc = _battleStartFunc;
	character = _characterDataManager;
	gameObjectManager = _gameObjectManager;
	isFirstTurn = true;
}

void BoardGamePart::Setup() {
	if (!stage)
		stage = gameObjectManager->Find("Stage");

	if (!camera)
		camera = gameObjectManager->Find("Camera");

	// 現在のターンのプレイヤーを取得
	currentTurnPlayerObject = gameObjectManager->Find(currentPlayerData->GetPlayerName());

	// このターン動けるかを確認
	if (!currentPlayerData->IsMove()) {
		// 動けないターン数を減らす
		currentPlayerData->SubDontMoveTurnNum();
		// ターンを終える
		turnState = TurnState_End;
		return;
	}

	// できるコマンドを探す
	PickUpCommand();

	// アイテム、魔法の使用可数を初期化
	currentPlayerData->ResetUsedItemCount();
	currentPlayerData->ResetUsedMagicCount();

	turnState = TurnState::TurnState_CommandSelect;

	battleTargetIndex = 0;

	// タイルの効果にプレイヤーを渡す
	TileEffect::SetCurrentTurnPlayer(currentPlayerData);

	// コマンド選択に行く前に同じマスに誰かがいればバトルをする
	// 最初のターンであればバトルは無視する
	if (!isFirstTurn)
		InBattle();
}


void BoardGamePart::Update(float _t) {
	// カメラの更新
	if (camera) {
		Vector3 playerPos = currentTurnPlayerObject->GetTransform()->GetPosition();
		Vector3 cameraPos = Vector3::VAdd(playerPos, Vector3::VScale(VUp, 1000));
		cameraPos.z -= 500;
		camera->GetTransform()->SetPosition(cameraPos);
		camera->GetTransform()->LookAt(playerPos);
	}

	switch (turnState) {
	case TurnState_CommandSelect:
		CommandSelect();
		break;
	case TurnState_Move:
		MoveCommand();
		break;
	case TurnState_Item:
		ItemCommand();
		break;
	case TurnState_Magic:
		MagicCommand();
		break;
	case TurnState_SearchTile:
		SearchTileCommand();
		break;
	case TurnState_TileEffect:
		TileEffectExecute();
		break;
	case TurnState_BattleIn:
		BattleCommand();
		break;
	case TurnState_End:
		TurnEnd();
		break;
	default:
		break;
	}

}

void BoardGamePart::Render() {
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
		canvas->SetSelectIndex(itemCommand.GetSelectItemIndex());
		canvas->OpenItemListUI();
		break;
	case TurnState_Magic:
		canvas->SetSelectIndex(magicCommand.GetSelectMagicIndex());
		canvas->OpenMagicListUI();
		break;
	case TurnState_SearchTile:
		searchTileCommand.Render();
		break;
	case TurnState_BattleIn:
	{
		int i = 0;
		unsigned int color = 0xffffff;
		for (auto player : canBattlePlayer) {
			color = (i == battleTargetIndex) ? 0xffff00 : 0x000000;
			DrawFormatString(400, 100 + i * 20, color, "%s", player->GetPlayerName().c_str());
			i++;
		}
	}
	break;
	case TurnState_End:

		break;
	default:
		break;
	}
}

void BoardGamePart::CommandSelect() {
	// 上
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
		selectCommand = max(0, selectCommand - 1);
	// 下
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
		selectCommand = min(static_cast<int>(commandList.size()) - 1, selectCommand + 1);
	// 選択
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
		turnState = commandList[selectCommand].state;
	}
}

void BoardGamePart::MoveCommand() {

	// 移動結果を受け取る
	MoveCommandState move = moveCommand.Execute();

	switch (move) {
	case MoveCommand_Back:
		// 移動をやめた
		turnState = TurnState_CommandSelect;
		break;
	case MoveCommand_Stay:
		// 移動中
		break;
	case MoveCommand_MoveEnd:
		// 移動終了
		// タイルの効果処理に入る
		turnState = TurnState_TileEffect;
		break;
	}
}

void BoardGamePart::ItemCommand() {
	if (itemCommand.SelectItem()) {
		turnState = TurnState_CommandSelect;
		canvas->CloseItemListUI();
	}

	PickUpCommand();
}

void BoardGamePart::MagicCommand() {
	if (magicCommand.SelectMagic()) {
		turnState = TurnState_CommandSelect;
		canvas->CloseMagicListUI();
	}

	PickUpCommand();
}

void BoardGamePart::BattleCommand() {
	int battleTargetNum = canBattlePlayer.size();

	if (battleTargetNum == 0) {
		turnState = TurnState_CommandSelect;
		return;
	}
	// 一人ならそのままバトルに入る
	if (battleTargetNum == 1) {
		battleStartFunc(currentPlayerData, canBattlePlayer.front());
		return;
	}

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
		battleTargetIndex = max(0, battleTargetIndex - 1);

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
		battleTargetIndex = min(battleTargetNum - 1, battleTargetIndex + 1);

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
		battleStartFunc(currentPlayerData, canBattlePlayer[battleTargetIndex]);
	}
}

void BoardGamePart::SearchTileCommand() {
	searchTileCommand.SearchTile();

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_SPACE))
		turnState = TurnState_CommandSelect;
}

void BoardGamePart::TurnEnd() {
	turnCount++;
	// 最初のターンフラグを落とす
	if (turnCount == 3)
		isFirstTurn = false;

	turnEndFunc();
}

void BoardGamePart::SetCurrentPlayerData(PlayerData* _currentPlayerData) {
	currentPlayerData = _currentPlayerData;
	itemCommand.SetCurrentTurnPlayer(_currentPlayerData);
	magicCommand.SetCurrentTurnPlayer(_currentPlayerData);
	moveCommand.SetCurrentTurnPlayer(_currentPlayerData);
}

void BoardGamePart::TileEffectExecute() {
	Vector3 pos = currentPlayerData->GetMapPosition();
	bool isGoNext = false;
	std::vector<CharacterData*> characters = character->GetCharacterDataToMapPos(pos.x, pos.y);
	TileEffectEnum effect = TileEffect::Effect(map->GetMapTileType(pos.x, pos.y), characters, isGoNext);

	// バトル
	if (isGoNext && effect == TileEffect_Battle) {
		// 今いるマスに自分しかいないなら敵を生成する
		if (characters.size() == 1)
			CharacterData* enemy = character->CreateEnemyData(0, pos.x, pos.y);

		// バトルに入る
		InBattle();

		turnCount++;
		// 最初のターンフラグを落とす
		if (turnCount == 3)
			isFirstTurn = false;

		// バトルに入ったら帰る
		if (turnState == TurnState_BattleIn)
			return;
	}

	if (isGoNext/* && InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)*/)
		turnState = TurnState_End;
}

void BoardGamePart::InBattle() {
	Vector3 pos = currentPlayerData->GetMapPosition();

	// バトルできる場所であればバトルする
	if (map->GetMapTileType(pos.x, pos.y) != Tile_Empty)
		return;

	// 同じマスにプレイヤーがいれば戦闘に入る
	auto array = character->GetCharacterDataToMapPos(pos.x, pos.y);
	if (array.empty()) {
		return;
	}

	canBattlePlayer.clear();

	// バトルできるプレイヤーを分ける
	for (auto& p : array) {
		// ターン中のプレイヤーなら次
		if (currentPlayerData == p) continue;
		PlayerData* pd = dynamic_cast<PlayerData*>(p);
		// 敵キャラクターか、戦えるプレイヤーなら候補に入れる
		if (!pd || (pd && pd->CanBattling()))
			canBattlePlayer.push_back(p);
	}
	// バトルできるプレイヤーがいるならバトルに入る
	if (canBattlePlayer.size() > 0) {
		turnState = TurnState_BattleIn;
		return;
	}
}

void BoardGamePart::PickUpCommand() {
	commandList.clear();
	commandList.push_back(Command(TurnState::TurnState_Move, "移動", "ルーレットを回して移動する"));
	moveCommand.Setup();
	// アイテムを持っていて使える回数が残っていれば選択できる
	if (currentPlayerData->IsUseItem()) {
		commandList.push_back(Command(TurnState::TurnState_Item, "アイテム", "持っているアイテムを確認、使用できる"));
	}
	if (currentPlayerData->IsUseMagic()) {
		commandList.push_back(Command(TurnState::TurnState_Magic, "魔法", "持っている魔法を確認、使用できる"));
	}
	commandList.push_back(Command(TurnState::TurnState_SearchTile, "確認", "マスを確認する"));

	selectCommand = 0;
}
