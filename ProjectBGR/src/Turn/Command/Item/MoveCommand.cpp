#include "MoveCommand.h"
#include "Manager/Map/MapManager.h"
#include "Data/Player/PlayerData.h"

void MoveCommand::Move() {
	if (!currentPlayerData) return;

	// 進める回数を超えていたら移動できない
	if (currentMoveCount >= canMoveCount) return;

	InputManager& inputManager = InputManager::GetInstance();

	if (canMoveTileList.empty()) return;

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

	if (!ismoved) return;

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
				return;
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
		// 進んだマスを削除
		movedTiles.pop_back();
		// 進んだ回数を減らす
		currentMoveCount--;
	}

	// 進める回数を超えたらターン終了
	if (currentMoveCount >= canMoveCount) {
		currentTurn++;
		if (currentTurn >= orderList.size())
			currentTurn = 0;

		TileEffect(currentPos);
		TurnStart();
	}

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
	default:
		break;
	}
}
