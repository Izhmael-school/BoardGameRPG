#include "MapManager.h"
#include "DxLib.h"
#include "imgui.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "Manager/PlayerData/PlayerDataManager.h"
#include "Data/Player/PlayerData.h"

MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}

void MapManager::Update(float _t)
{
}

void MapManager::Render() {
	for (int i = 0; i < mapList.size(); ++i) {
		for (int j = 0; j < mapList[i].size(); ++j) {
			switch (static_cast<MapTileType>(mapList[i][j])){
			case MapTileType::Tile_Wall:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 0, 255), TRUE);
				break;
			case MapTileType::Tile_Road:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(255,255, 0), TRUE);
				break;
			case MapTileType::Tile_Empty:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 255, 0), TRUE);
				break;
			case MapTileType::Tile_Item:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(255, 0, 255), TRUE);
				break;
			case MapTileType::Tile_Magic:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 255, 255), TRUE);
				break;
			case MapTileType::Tile_ItemShop:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(255, 255, 0), TRUE);
				break;
			case MapTileType::Tile_MagicShop:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(255, 0, 0), TRUE);
				break;
			case MapTileType::Tile_EquipShop:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 0, 255), TRUE);
				break;
			case MapTileType::Tile_Village:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 255, 0), TRUE);
				break;
			case MapTileType::Tile_Church:
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(255, 255, 255), TRUE);
				break;
			default:
				break;
			}
		}
	}

	PlayerDataManager& p = PlayerDataManager::GetInstance();

	int playerNum = p.GetPlayerNum();
	for (int i = 0; i < playerNum;i++) {
		PlayerData* data = p.GetPlayerData(i);
		Vector3 mapPos = data->GetMapPosition();
		int x = mapPos.x;
		int y = mapPos.y;

		DrawCircle(x * 32 + 16, y * 32 + 16, 10, data->GetColor());
	}
}

void MapManager::LoadMap() {
	auto mapData = MyJson::LoadJsonFile("res/ExternalFile/Map/map.json");
	for (auto& data : mapData) {
		auto& tile = data["tiles"];
		int mapHeight = tile.size();
		int mapWidth = tile[0].size();
		mapList.resize(mapWidth);
		for (int i = 0; i < mapHeight; ++i) {
			int mapWidth = tile[i].size();
			for (int j = 0; j < mapWidth; ++j) {
				mapList[j].push_back(static_cast<int>(tile[i][j]));
			}
		}
	}
}
