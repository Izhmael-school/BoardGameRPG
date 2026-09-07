#include "MoveCommand.h"
#include "Manager/Map/MapManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Input/InputManager.h"
#include "Manager/PlayerData/PlayerDataManager.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "DxLib.h"
#include <string>

bool MoveCommand::Move() {
	if (!currentPlayerData) return true;

	// 進める回数を超えていたら移動できない
	if (currentMoveCount >= canMoveCount) return true;

	InputManager& inputManager = InputManager::GetInstance();

	// 進めるマスを調べる
	SearchCanMoveTiles();

	if (canMoveTileList.empty()) return true;

	bool ismoved = false;
	Vector3 movedDir = Vector3(0, 0);
	Vector3 beforeMoveTile = Vector3(-1, -1);
	Vector3 movedir = Vector3(0, 0);

	if (canMoveTileList[Move_Down] && inputManager.IsKeyDown(KEY_INPUT_DOWN)) {
		movedir = Vector3(0, 1);
		ismoved = true;
	}
	if (canMoveTileList[Move_Up] && inputManager.IsKeyDown(KEY_INPUT_UP)) {
		movedir = Vector3(0, -1);
		ismoved = true;
	}
	if (canMoveTileList[Move_Left] && inputManager.IsKeyDown(KEY_INPUT_LEFT)) {
		movedir = Vector3(-1, 0);
		ismoved = true;
	}
	if (canMoveTileList[Move_Right] && inputManager.IsKeyDown(KEY_INPUT_RIGHT)) {
		movedir = Vector3(1, 0);
		ismoved = true;
	}

	if (!ismoved) return false;

	beforeMoveTile = currentPlayerData->GetMapPosition();
	currentPlayerData->AddMapPosition(movedir);
	ismoved = true;
	movedDir = movedir;

	// 進んだ先のタイルIDが道だった場合は同じ方向にもう一度進む
	bool isMovedAgain = false;
	do {
		isMovedAgain = false;
		Vector3 mapPos = currentPlayerData->GetMapPosition();
		int x = mapPos.x;
		int y = mapPos.y;
		if (mapManager->GetMapList()[x][y] == static_cast<int>(MapTileType::Tile_Road)) {
			// 次のマスが道かマスでなければ進めない
			if (mapManager->GetMapList()[x + movedir.x][y + movedir.y] == static_cast<int>(MapTileType::Tile_Wall)) {
				currentPlayerData->SetMapPosition(beforeMoveTile);
				return false;
			}

			isMovedAgain = true;
			currentPlayerData->AddMapPosition(movedDir);
		}
	} while (isMovedAgain);

	// 道を戻っていたら戻ったことにする
	Vector3 prevPos = Vector3(-1, -1);
	if (!movedTiles.empty())
		prevPos = movedTiles.back();
	Vector3 currentPos = currentPlayerData->GetMapPosition();
	// 戻っていない
	if (prevPos.x != currentPos.x || prevPos.y != currentPos.y) {
		//進んだマスを記録
		movedTiles.push_back(beforeMoveTile);
		// 進んだ回数を増やす
		currentMoveCount++;
	}
	// 戻っている
	else {
		Back();
	}

	// 進める回数を超えたらターン終了
	if (currentMoveCount >= canMoveCount) {
		TileEffect(currentPos);
		return true;
	}

	return false;
}

void MoveCommand::Back() {
	if (movedTiles.empty()) return;

	Vector3 back = movedTiles.back();
	movedTiles.pop_back();

	currentPlayerData->SetMapPosition(back);
	currentMoveCount--;
}

void MoveCommand::SearchCanMoveTiles() {
	if (!currentPlayerData || !mapManager) return;

	Vector3 mapPos = currentPlayerData->GetMapPosition();
	int x = mapPos.x;
	int y = mapPos.y;

	const MapTileArray& mapList = mapManager->GetMapList();

	std::vector<bool> canMoveList;

	// 上
	if (y > 0 && mapList[x][y - 1] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	// 下
	if (y < static_cast<int>(mapList.size()) - 1 && mapList[x][y + 1] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	// 左
	if (x > 0 && mapList[x - 1][y] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	// 右
	if (x < static_cast<int>(mapList[y].size()) - 1 && mapList[x + 1][y] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	canMoveTileList = canMoveList;
}

void MoveCommand::TileEffect(Vector3 _tilePosition) {
	int tileId = mapManager->GetMapList()[_tilePosition.x][_tilePosition.y];
	switch (static_cast<MapTileType>(tileId)) {
	case MapTileType::Tile_Item:
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		currentPlayerData->AddItem(0);
		break;
	case MapTileType::Tile_Magic:
		currentPlayerData->AddMagic(0);
		currentPlayerData->AddMagic(0);
		currentPlayerData->AddMagic(0);
		currentPlayerData->AddMagic(0);
		currentPlayerData->AddMagic(0);
		break;
	case MapTileType::Tile_ItemShop:
		break;
	case MapTileType::Tile_MagicShop:
		break;
	case MapTileType::Tile_EquipShop:
		break;
	case MapTileType::Tile_Village:
		break;
	case MapTileType::Tile_Church:
		break;
	case MapTileType::Tile_Empty:
		// 同じマスにプレイヤーがいれば戦闘に入る
		auto array = PlayerDataManager::GetInstance().GetPlayerDataToMapPos(_tilePosition.x, _tilePosition.y);
		if (array.empty()) break;

		std::vector<PlayerData*> canBattlePlayer;

		// バトルできるプレイヤーを分ける
		for (auto& p : array) {
			if (currentPlayerData != p)
				canBattlePlayer.push_back(p);
		}
		break;
	}
}

bool MoveCommand::Roulette() {

	randCount = GetRand(6);

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
		canMoveCount = randCount;
		return true;
	}

	return false;
}

bool MoveCommand::ConfirmationMove() {
	// 戻る
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
		confirmationMove = 0;

	// 止まる
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
		confirmationMove = 1;

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
		if (confirmationMove == 1)
			return true;
		else {
			Back();
			commandState = MoveCommand_Move;
		}
	}

	return false;
}

MoveCommandState MoveCommand::Execute() {
	// ルーレットを回してなければ戻る
	if (isBack && InputManager::GetInstance().IsKeyDown(KEY_INPUT_B))
		return commandState = MoveCommand_Back;

	// ルーレットを回す
	// 確定したらコマンド選択に戻れないようにする
	if (isBack && Roulette()) {
		isBack = false;
		return	commandState = MoveCommand_Move;
	}

	if (commandState == MoveCommand_Stay)
		return commandState = MoveCommand_Stay;

	if (Move()) {
		commandState = MoveCommand_Confirmation;
	}

	if (ConfirmationMove())
		return commandState = MoveCommand_MoveEnd;

	return commandState;
}

void MoveCommand::Setup() {
	isBack = true;
	canMoveCount = 0;
	currentMoveCount = 0;
	movedTiles.clear();
	commandState = MoveCommand_Stay;
}

void MoveCommand::Render() {
	VECTOR offset = VGet(400, 200, 0);
	VECTOR offset2 = VGet(440, 240, 0);
	int stringSize = 40;

	// ウィンドウ
	DrawFillBox(offset.x, offset.y, offset2.x, offset2.y, 0x000000);
	DrawLineBox(offset.x, offset.y, offset2.x, offset2.y, 0xffffff);

	int extend = 2.0f;
	int sx;
	int sy;
	GetDrawExtendStringSize(&sx, &sy, 0, extend * 2, extend, "1", 1);
	int x = (offset2.x - (offset2.x - offset.x) / 2) - sx / 2;
	int y = (offset2.y - (offset2.y - offset.y) / 2) - sy / 2;

	int correctionX = stringSize - sx;
	int correctionY = stringSize - sy;

	DrawExtendFormatString(x + correctionX, y + correctionY, extend * 2, extend, 0xffff00, "%d", isBack ? randCount : canMoveCount - currentMoveCount);

	// 移動を確定するかのウィンドウ
	if (commandState != MoveCommand_Confirmation) return;

	stringSize = 18;
	offset = VGet(400, 240, 0);
	offset2 = VGet(454, 276, 0);

	// ウィンドウ
	DrawFillBox(offset.x, offset.y, offset2.x, offset2.y, 0x000000);
	DrawLineBox(offset.x, offset.y, offset2.x, offset2.y, 0xffffff);
	std::string back = "戻る";
	std::string stop = "止まる";
	DrawString(offset.x, offset.y, MyJson::Utf8ToString(back).c_str(), confirmationMove == 0 ? 0xffff00 : 0xffffff);
	DrawString(offset.x, offset.y + stringSize, MyJson::Utf8ToString(stop).c_str(), confirmationMove == 1 ? 0xffff00 : 0xffffff);
}
