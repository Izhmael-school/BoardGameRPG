#include "MapManager.h"
#include "DxLib.h"
#include "imgui.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include "Data/Player/PlayerData.h"

MapManager::MapManager(){
	LoadMap();
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

	for (auto p : canMoveTile) {
		DrawBox(p.x * 32, p.y * 32, (p.x + 1) * 32, (p.y + 1) * 32, GetColor(255, 255, 255), TRUE);
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

MapTileType MapManager::GetMapTileType(int x, int y) {
	return static_cast<MapTileType>(mapList[x][y]);
}

#include <queue>
#include <tuple>
std::vector<Vector3> MapManager::GetCanMoveTile(int _x, int _y, int _canMoveCount) {
    std::vector<Vector3> result;

    int width = static_cast<int>(mapList.size());
    if (width == 0) return result;
    int height = static_cast<int>(mapList[0].size());

    std::vector<std::vector<int>> visited(width, std::vector<int>(height, -1));
    visited[_x][_y] = 0;

    std::queue<std::tuple<int, int, int>> q; // x, y, cost
    q.push({ _x, _y, 0 });

    const int dx[4] = { 0, 0, -1, 1 };
    const int dy[4] = { -1, 1, 0, 0 };

    auto inBounds = [&](int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
        };
    auto isWall = [&](int x, int y) {
        return mapList[x][y] == static_cast<int>(MapTileType::Tile_Wall);
        };
    auto isRoad = [&](int x, int y) {
        return mapList[x][y] == static_cast<int>(MapTileType::Tile_Road);
        };
    // 「止まれるマス」= 壁でもRoadでもないマス
    auto isStoppable = [&](int x, int y) {
        return !isWall(x, y) && !isRoad(x, y);
        };

    while (!q.empty()) {
        auto [cx, cy, cost] = q.front();
        q.pop();

        if (cost >= _canMoveCount) continue;

        for (int dir = 0; dir < 4; dir++) {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];

            if (!inBounds(nx, ny) || isWall(nx, ny)) continue;
            if (visited[nx][ny] != -1) continue;

            // Roadマスなら同方向にスライドし続ける
            int fx = nx, fy = ny;
            while (isRoad(fx, fy)) {
                int sx = fx + dx[dir];
                int sy = fy + dy[dir];

                if (!inBounds(sx, sy) || isWall(sx, sy))
                    break; // 壁で阻まれてRoad上で強制停止(この場合も止まれるマスではない)

                if (visited[fx][fy] == -1) {
                    visited[fx][fy] = cost + 1; // 探索は続けるが結果には入れない
                }

                fx = sx;
                fy = sy;
            }

            if (visited[fx][fy] != -1) continue;

            visited[fx][fy] = cost + 1;
            q.push({ fx, fy, cost + 1 });

            // 止まれるマスだけを結果に追加
            if (isStoppable(fx, fy)) {
                result.push_back(Vector3(static_cast<float>(fx), static_cast<float>(fy), 0.0f));
            }
        }
    }

	canMoveTile = result;

    return result;
}
