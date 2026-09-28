/*
 * @brief 繝槭ャ繝励・隱ｭ縺ｿ霎ｼ縺ｿ繧・ｮ｡逅・ｒ陦後≧
 * @author Sekino
 */

#pragma once
#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

#include "../ManagerBase.h"
#include <vector>
#include "Vector3.h"

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
	MapTileArray mapList; // 繝槭ャ繝励・繝ｪ繧ｹ繝・
	std::vector<Vector3> canMoveTile;

public:
	MapManager();
	~MapManager();

public:
	void Update(float _t) override;;

	void Render() override;

	void LoadMap();

	MapTileArray& GetMapList() { return mapList; }

	MapTileType GetMapTileType(int x, int y);

	/*
	 * @brief 現在のマスと動けるマスを使ってどのマスまで行けるのかを探す
	 */
	std::vector<Vector3> GetCanMoveTile(int _x, int _y, int _canMoveCount);
};

#endif // !_MAPMANAGER_H_