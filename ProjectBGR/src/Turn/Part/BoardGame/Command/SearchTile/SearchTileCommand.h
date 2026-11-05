/*
 * @brief マスの効果やマスにいるキャラクターの情報を確認する
 * @author Sekino
 */

#pragma once
#ifndef _SEARCHTILECOMMAND_H_
#define _SEARCHTILECOMMAND_H_

class MapManager;
class CharacterDataManager;
class GameObject;
class CharacterData;

#include "Vector2.h"
#include <vector>
#include "Manager/Map/MapInfo.h"

class SearchTileCommand {
private:
	int currentX, currentY;
	int mapMaxX, mapMaxY;
	MapManager* map;
	CharacterDataManager* character;

	GameObject* mapArrow;

public:
	void Init(MapManager* _map,CharacterDataManager* _character,GameObject* _mapArrow);

	/*
	 * @brief 情報を確認するマスを決める
	 */
	void SearchTile();

	void Render();

	Vector2 GetCurrentPosition() const { return Vector2(currentX, currentY); }

	void Cleanup();

	void SetCurrentPosition(int _x, int _y) { currentX = _x; currentY = _y; }

	MapTileType GetCurrentTileType() const;

	std::vector<CharacterData*> GetCurrentCharacters() const;
private:
};

#endif // !_SEARCHTILECOMMAND_H_