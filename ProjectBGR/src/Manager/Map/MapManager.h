/*
 * @brief マップの読み込みや管理を行う
 * @author Sekino
 */

#pragma once
#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

#include "../ManagerBase.h"
#include <vector>

enum MapTileType {
	Tile_Wall = 0,
	Tile_Road = 1,
	Tile_Empty = 2,
	Tile_Item = 3,
	Tile_Magic = 4,
	Tile_ItemShop = 5,
	Tile_MagicShop = 6,
	Tile_EquipShop = 7,
	Tile_Village = 8,
	Tile_Church = 9
};

using MapTileArray = std::vector<std::vector<int>>;
class MapManager : public ManagerBase{
private:
	MapTileArray mapList; // マップのリスト

public:
	MapManager();
	~MapManager();

public:
	void Update(float _t) override;;

	void Render() override;

	void LoadMap();

	MapTileArray& GetMapList() { return mapList; }
};

#endif // !_MAPMANAGER_H_