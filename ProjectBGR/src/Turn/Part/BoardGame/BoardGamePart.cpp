#include "BoardGamePart.h"
#include "Manager/Input/InputManager.h"
#include "DxLib.h"
#include "Data/Player/PlayerData.h"
#include "Data/CharacterData.h"
#include "Data/Enemy/EnemyData.h"
#include "Manager/Map/MapManager.h"
#include "Effect/Tile/TileEffect.h"
#include "Effect/Item/ItemEffect.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include "GameObject/GameObject.h" 
#include "Manager/GameObject/GameObjectManager.h"
#include "UI/Canvas/SceneCanvas/BoardGame/BoardGameCanvas.h"
#include "Application.h"
#include "Manager/Resource/Sprite/SpriteResourceManager.h"

void BoardGamePart::Init(UIManager* _uiManager, MapManager* _map, CharacterDataManager* _characterDataManager, std::function<void()> _turnEndFunc, std::function<void(CharacterData*, CharacterData*)> _battleStartFunc, GameObjectManager* _gameObjectManager, BoardGameCanvas* _canvas) {
	canvas = _canvas;
	moveCommand.SetMapManager(_map);
	map = _map;
	turnEndFunc = _turnEndFunc;
	battleStartFunc = _battleStartFunc;
	character = _characterDataManager;
	gameObjectManager = _gameObjectManager;
	searchTileCommand.Init(_map, _characterDataManager, gameObjectManager->UseObject<GameObject>("MapArrow", VZero, VZero, VOne, "MapArrow"));
	magicCommand.SetCharacterDataManager(character);
	isFirstTurn = true;
	magicCommand.Init();
	TileEffect::SetChangeTurnStateFunc([this](TurnState _changeTurnState) {turnState = _changeTurnState; });
	TileEffect::SetSetBoardGameUINeedInfoFunc([this](BoardGameUINeedInfo _info) {canvas->SetBoardGameUINeedInfo(_info); });
	// アイテム効果実行クラスにイベントを渡す
	ItemAndMagicEffect::SetFixedMoveFunc([this](int _moveNum) {moveCommand.FixedMove(_moveNum); turnState = TurnState_Move; });
	ItemAndMagicEffect::SetRandomWarpFunc([this](Vector2 _beforePos) {return map->GetRandomPos(_beforePos); });
	ItemAndMagicEffect::SetMoveEndFunc([this]() {MoveEndForItem(); });

	shopPart.Init(_uiManager, [this]() { ShopOut(); });
	churchPart.Init(_uiManager, [this]() { ShopOut(); });
	villagePart.Init(_uiManager, [this]() { ShopOut(); });

	canvas->SetTurnUIContext(TurnState_Magic, magicCommand.GetUIContext());

	// 最初にすべての村に敵を配置する
	character->CreateEnemyDataToAllVillage();
}

void BoardGamePart::Setup() {
	if (!stage)
		stage = gameObjectManager->Find("Stage");

	if (!camera)
		camera = gameObjectManager->Find("Camera");

	auto a = gameObjectManager->Find("MapArrow");

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

	moveCommand.Setup();
}

MapTileType BoardGamePart::GetCurrentPlayerMapTileType() {
	Vector2 pos = currentPlayerData->GetMapPosition();
	return map->GetMapTileType(pos.x, pos.y);
}


void BoardGamePart::Update(float _t) {
	// カメラの更新
	if (camera) {
		Vector3 lookPos = currentTurnPlayerObject->GetTransform()->GetPosition();
		Vector3 cameraPos = Vector3::VAdd(lookPos, Vector3::VScale(VUp, 1000));

		if (turnState == TurnState_SearchTile) {
			Vector2 pos = searchTileCommand.GetCurrentPosition();
			lookPos = Vector3(pos.x * 200, 0, pos.y * -200);
			cameraPos = Vector3::VAdd(lookPos, Vector3::VScale(VUp, 1000));
		}
		cameraPos.z -= 500;

		camera->GetTransform()->SetPosition(cameraPos);
		camera->GetTransform()->LookAt(lookPos);
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
	case TurnState_ShopIn:
		shopPart.Update(_t);
		break;
	case TurnState_ChurchIn:
		churchPart.Update(_t);
		break;
	case TurnState_VillageIn:
		villagePart.Update(_t);
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
	/*auto sprite = Application::GetInstance().GetSpriteResourceManager();
	DrawGraph(0, 0, sprite->GetSprite("ItemShop"), TRUE);*/
}

void BoardGamePart::CommandSelect() {
	InputManager* input = &InputManager::GetInstance();
	int port = currentPlayerData->GetControllerNum();

	// 上
	if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(port, XINPUT_DPAD_UP))
		selectCommand = max(0, selectCommand - 1);
	// 下
	if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(port, XINPUT_DPAD_DOWN))
		selectCommand = min(static_cast<int>(commandList.size()) - 1, selectCommand + 1);
	// 選択
	if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonDown(port, XINPUT_A)) {
		turnState = commandList[selectCommand].state;
	}

	CommandSelectUINeedInfo commandSelectInfo;
	commandSelectInfo.commandList = commandList;
	commandSelectInfo.selectIndex = selectCommand;

	BoardGameUINeedInfo info;
	info.state = turnState;
	info.info = commandSelectInfo;

	canvas->SetBoardGameUINeedInfo(info);
}

void BoardGamePart::MoveCommand() {

	// 移動結果を受け取る
	MoveCommandState move = moveCommand.Execute();

	switch (move) {
	case MoveCommand_Back:
		// 移動をやめた
		turnState = TurnState_CommandSelect;
		break;
	case MoveCommand_RouletteTime:
		// 移動中
		break;
	case MoveCommand_MoveEnd:
		// 移動終了
		// タイルの効果処理に入る
		turnState = TurnState_TileEffect;
		break;
	}

	MoveCommandUINeedInfo rouletteinfo;
	rouletteinfo.commandState = move;
	rouletteinfo.angle = moveCommand.GetRoletteAngle();
	rouletteinfo.isRouletteStop = moveCommand.IsRouletteStop();
	rouletteinfo.canMoveCount = moveCommand.GetCanMoveCount() - moveCommand.GetCurrentMoveCount();
	rouletteinfo.isDialogTextErase = moveCommand.IsDialogTextDelete();
	rouletteinfo.confirmationMove = moveCommand.GetConfirmationMove();
	rouletteinfo.rouletteNum = moveCommand.GetRouletteNum();

	BoardGameUINeedInfo info;
	info.state = turnState;
	info.info = rouletteinfo;

	canvas->SetBoardGameUINeedInfo(info);
}

void BoardGamePart::ItemCommand() {
	if (itemCommand.SelectItem()) {

		static int i = -1;

		if (InputManager::GetInstance().IsButtonDown(currentPlayerData->GetControllerNum(), XINPUT_A))
			i++;

		if (itemCommand.IsCancel())
			i = 2;

		switch (i) {
		case 1:
			if (!itemCommand.IsUsed())
				// アイテムを使う
				itemCommand.UseItem();
			break;
		case 2:
			// 使ったアイテムを消す
			if (!itemCommand.IsCancel())
				itemCommand.RemoveUsedItem();
			// 戻る
			// アイテムの効果で状態が変わってなければ
			if (turnState == TurnState_Item)
				turnState = TurnState_CommandSelect;
			canvas->CloseItemListUI();
			PickUpCommand();
			itemCommand.SetCancel(false);
			break;
		}
	}

	ItemCommandUINeedInfo itemInfo;
	itemInfo.items = currentPlayerData->GetItemList();
	itemInfo.currentSelect = itemCommand.GetSelectItemIndex();
	itemInfo.isSelect = itemCommand.IsSelect();
	itemInfo.isUsed = itemCommand.IsUsed();
	itemInfo.usedItemAnyInfo = itemCommand.GetUsedItemAnyInfo();

	BoardGameUINeedInfo info;
	info.state = turnState;
	info.info = itemInfo;

	canvas->SetBoardGameUINeedInfo(info);
}

void BoardGamePart::MagicCommand() {
	if (magicCommand.SelectMagic()) {
		static int i = 0;

		if (InputManager::GetInstance().IsButtonDown(currentPlayerData->GetControllerNum(), XINPUT_A))
			i++;

		if (magicCommand.IsCancel())
			i = 2;

		switch (i) {
		case 1:
			if (!magicCommand.IsUsed())
				// アイテムを使う
				magicCommand.UseMagic();
			break;
		case 2:
			// 使ったアイテムを消す
			if (!magicCommand.IsCancel())
				magicCommand.RemoveUsedMagic();
			// 戻る
			turnState = TurnState_CommandSelect;
			PickUpCommand();
			magicCommand.SetCancel(false);
			i = 0;
			break;
		}
	}

	MagicCommandUINeedInfo magicInfo;
	magicInfo.magics = currentPlayerData->GetMagicList();
	magicInfo.currentSelect = magicCommand.GetSelectMagicIndex();
	magicInfo.currentSelectTarget = magicCommand.GetSelectTargetIndex();
	//magicInfo.isSelect = 
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

	InputManager* input = &InputManager::GetInstance();
	int port = currentPlayerData->GetControllerNum();

	if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(port, XINPUT_DPAD_UP))
		battleTargetIndex = max(0, battleTargetIndex - 1);

	if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(port, XINPUT_DPAD_DOWN))
		battleTargetIndex = min(battleTargetNum - 1, battleTargetIndex + 1);

	if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonDown(port, XINPUT_A)) {
		battleStartFunc(currentPlayerData, canBattlePlayer[battleTargetIndex]);
	}

	BattleInUINeedInfo battleInInfo;
	battleInInfo.battleTargets = canBattlePlayer;
	battleInInfo.selectIndex = battleTargetIndex;

	BoardGameUINeedInfo info;
	info.state = turnState;
	info.info = battleInInfo;

	canvas->SetBoardGameUINeedInfo(info);
}

void BoardGamePart::SearchTileCommand() {
	static bool isSetup = false;

	if (!isSetup) {
		searchTileCommand.SetCurrentPosition(currentPlayerData->GetMapPosition().x, currentPlayerData->GetMapPosition().y);
		isSetup = true;
	}

	searchTileCommand.SearchTile();

	InputManager* input = &InputManager::GetInstance();

	if (input->IsButtonDown(currentPlayerData->GetControllerNum(), XINPUT_B)) {
		searchTileCommand.Cleanup();
		turnState = TurnState_CommandSelect;
		isSetup = false;
	}

	SearchTileUINeedInfo searchTileInfo;
	searchTileInfo.tileType = searchTileCommand.GetCurrentTileType();
	searchTileInfo.characters = searchTileCommand.GetCurrentCharacters();

	BoardGameUINeedInfo info;
	info.state = turnState;
	info.info = searchTileInfo;

	canvas->SetBoardGameUINeedInfo(info);
}

void BoardGamePart::TurnEnd() {
	turnCount++;
	// 最初のターンフラグを落とす
	if (turnCount == 3)
		isFirstTurn = false;

	BoardGameUINeedInfo info;
	info.state = TurnState_End;

	canvas->SetBoardGameUINeedInfo(info);

	turnEndFunc();
}

void BoardGamePart::SetCurrentPlayerData(PlayerData* _currentPlayerData) {
	currentPlayerData = _currentPlayerData;
	itemCommand.SetCurrentTurnPlayer(_currentPlayerData);
	magicCommand.SetCurrentTurnPlayer(_currentPlayerData);
	moveCommand.SetCurrentTurnPlayer(_currentPlayerData);
}

void BoardGamePart::TileEffectExecute() {
	Vector2 pos = currentPlayerData->GetMapPosition();
	bool isGoNext = false;
	std::vector<CharacterData*> characters = character->GetCharacterDataToMapPos(pos.x, pos.y);
	TileEffectEnum effect = TileEffect::Effect(map->GetMapTileType(pos.x, pos.y), characters, isGoNext);

	if (!isGoNext) return;

	switch (effect) {
	case TileEffect_Battle:
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

		// バトルに入らなければそのままターンエンド
		turnState = TurnState_End;
		return;
	case TileEffect_ItemShop:
	case TileEffect_EquipShop:
	case TileEffect_MagicShop:
		shopPart.Open(pos.x, pos.y, currentPlayerData);
		return;
	case TileEffect_Village:
		villagePart.Enter(currentPlayerData);
		return;
	case TileEffect_Church:
		churchPart.Open(currentPlayerData);
		return;
	case TileEffect_Magic:
	case TileEffect_Item:
	default:
		turnState = TurnState_End;
		break;
	}
}

void BoardGamePart::InBattle() {
	Vector2 pos = currentPlayerData->GetMapPosition();

	MapTileType tileEffect = map->GetMapTileType(pos.x, pos.y);

	// バトルできる場所であればバトルする
	if (tileEffect != Tile_Empty && tileEffect != Tile_Village)
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

	// アイテムを持っていて使える回数が残っていれば選択できる
	if (currentPlayerData->IsUseItem()) {
		commandList.push_back(Command(TurnState::TurnState_Item, "道具", "持っている道具を確認、使用できる"));
	}
	if (currentPlayerData->IsUseMagic()) {
		commandList.push_back(Command(TurnState::TurnState_Magic, "魔法", "持っている魔法を確認、使用できる"));
	}
	commandList.push_back(Command(TurnState::TurnState_SearchTile, "確認", "マスを確認する"));

	selectCommand = 0;
}

void BoardGamePart::ShopIn() {
	turnState = TurnState_ShopIn;
	shopPart.Open(currentPlayerData->GetMapPosition().x, currentPlayerData->GetMapPosition().y, currentPlayerData);
}

void BoardGamePart::ShopOut() {
	// ショップを出たらそのままターンエンド 
	turnState = TurnState_End;
}

void BoardGamePart::MoveEndForItem() {
	turnState = TurnState_TileEffect;
}
