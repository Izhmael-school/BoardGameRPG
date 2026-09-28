#include "SearchTileCommand.h"
#include "Manager/Input/InputManager.h"
#include "Manager/Map/MapManager.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include "DxLib.h"
#include "Data/CharacterData.h"

void SearchTileCommand::Init(MapManager* _map, CharacterDataManager* _character) {
	map = _map;
	character = _character;

	MapTileArray& mapTile = map->GetMapList();
	mapMaxY = mapTile.size() - 1;
	mapMaxX = mapTile[0].size() - 1;
}

void SearchTileCommand::SearchTile() {
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
		currentY = max(0, currentY - 1);
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
		currentY = min(mapMaxY, currentY + 1);
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_LEFT))
		currentX = max(0, currentX - 1);
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RIGHT))
		currentX = min(mapMaxX, currentX + 1);
}

void SearchTileCommand::Render() {
	DrawLineBox(currentX * 32, currentY * 32, (currentX + 1) * 32, (currentY + 1) * 32, 0xffff00);

	MapTileType tile = map->GetMapTileType(currentX,currentY);
	switch (tile) {
	case Tile_Empty:
		printfDx("何もないマス。戦闘やイベントが起こるかも？");
		break;
	case Tile_Item:
		printfDx("ランダムでアイテムがもらえるマス。");
		break;
	case Tile_Magic:
		printfDx("ランダムで魔法がもらえるマス。");
		break;
	case Tile_ItemShop:
		break;
	case Tile_MagicShop:
		break;
	case Tile_EquipShop:
		break;
	case Tile_Village:
		break;
	case Tile_Church:
		break;
	default:
		break;
	}

	std::vector<CharacterData*> characters = character->GetCharacterDataToMapPos(currentX, currentY);
	// 指定のマスにキャラクターがいればキャラクターの情報を描画する
	if (characters.empty()) return;
}
